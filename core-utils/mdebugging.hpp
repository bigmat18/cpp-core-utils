#pragma once

#include <concepts>
#include <iostream>
#include <meta>
#include <string_view>
#include <source_location>
#include <thread>
#include <tuple>
#include <print>

namespace core::debug {

template<typename T>
concept Debuggable = requires(T val)
{
    val.first;
    val.second;
} && std::convertible_to<decltype(std::declval<T>().first), std::string_view>
  && std::formattable<std::remove_cvref_t<decltype(std::declval<T>().second)>, char>;

template <Debuggable... Args>
class breakpoint {
    bool m_Active = true;
    bool m_Debug = false;
    bool m_Condition = true;
    std::string_view m_Msg = "";
    std::source_location m_Location;
    std::tuple<const Args&...> m_Vars;

    template <Debuggable... OtherArgs>
    constexpr explicit breakpoint(
        bool debug, bool condition, std::string_view msg,
        std::source_location location, std::tuple<const OtherArgs&...> vars
    ) noexcept : m_Debug(debug), m_Condition(condition),
                 m_Msg(msg), m_Location(location), m_Vars(vars) {}

    template <Debuggable...>
    friend class breakpoint;

public:
    [[nodiscard]] constexpr explicit breakpoint(
        std::source_location location = std::source_location::current()
    ) noexcept : m_Location(location), m_Vars() {};

    breakpoint(breakpoint&&) noexcept = default;
    breakpoint& operator=(breakpoint&&) = delete;
    breakpoint(const breakpoint&) = delete;
    breakpoint& operator=(const breakpoint&) = delete;

    ~breakpoint() {
        if (!m_Active) {
            return;
        }

        if (m_Debug) {
#ifndef DEBUG
            return;
#endif // !DEBUG
        }

        if (m_Condition) {
            std::cerr << "[BREAKPOINT on thread " << std::this_thread::get_id() << "]\n";

            std::cerr << "\t[Stacktrace]" << "\n";
            std::cerr << "\t\tFile: " << m_Location.file_name() << ":"
                                      << m_Location.line() << ":" 
                                      << m_Location.column() << "\n";

            std::cerr << "\t\tFunction: " << m_Location.function_name() << "\n";
            if (m_Msg != "") {
                std::cerr << "\t\tMsg: " << m_Msg << "\n";
            }

            std::cerr << "\t[Variables]\n";
            template for (const auto& [name, val] : m_Vars) {
                using CleanType = std::remove_cvref_t<decltype(val)>;
                constexpr auto type_info = std::meta::dealias(^^CleanType);
                constexpr std::string_view type_name = std::meta::display_string_of(type_info);

                std::println(stderr, "\t\t{} {} [{:p}] = {}",
                    type_name,
                    name,
                    static_cast<const void*>(std::addressof(val)),
                    val
                );
            }

            std::cerr << "\n[Press Enter to continue]" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    constexpr auto if_debug() && noexcept {
        m_Active = false;
        return breakpoint(true, m_Condition, m_Msg, m_Location, m_Vars);
    }

    constexpr auto when(bool condition) && noexcept {
        m_Active = false;
        return breakpoint(m_Debug, m_Condition && condition, m_Msg, m_Location, m_Vars);
    }

    constexpr auto msg(std::string_view msg) && noexcept {
        m_Active = false;
        return breakpoint(m_Debug, m_Condition, msg, m_Location, m_Vars);
    }

    template <Debuggable... NewArgs>
    constexpr auto print_vars(const NewArgs&&... vars) && noexcept {
        m_Active = false;
        return breakpoint<NewArgs...>(
            m_Debug, m_Condition, m_Msg, m_Location, std::forward_as_tuple(vars...)
        );
    }
};

#define VAR(var) (std::pair<std::string_view, const decltype(var)&>{ #var, (var) })

} // core::debug


