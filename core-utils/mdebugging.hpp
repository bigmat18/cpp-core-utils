#pragma once

#include <iostream>
#include <meta>
#include <string_view>
#include <source_location>
#include <stacktrace>
#include <thread>
#include <tuple>
#include <print>

#include <common.hpp>

namespace core {

template <detail::Printable... Args>
class breakpoint {
    bool m_Active = true;
    bool m_Debug = false;
    bool m_Condition = true;
    std::string_view m_Msg = "";
    std::source_location m_Location;
    std::tuple<const Args&...> m_Vars;

    template <detail::Printable... OtherArgs>
    constexpr explicit breakpoint(
        bool debug, bool condition, std::string_view msg,
        std::source_location location, std::tuple<const OtherArgs&...> vars
    ) noexcept : m_Debug(debug), m_Condition(condition),
                 m_Msg(msg), m_Location(location), m_Vars(vars) {}

    template <detail::Printable...>
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
            std::println(stderr,
                    "\n" CORE_CLR_BOLD_RED  "[ BREAKPOINT ]" CORE_CLR_RESET "\n"
                    "  " CORE_CLR_BOLD_CYAN  "--- Information ---" CORE_CLR_RESET "\n"
                    "    " CORE_CLR_BOLD_WHITE "Thread ID : " CORE_CLR_RESET "{}\n"
                    "    " CORE_CLR_BOLD_WHITE "Location  : " CORE_CLR_RESET "{}:{}:{} in {}\n"
                    "    " CORE_CLR_BOLD_WHITE "Message   : " CORE_CLR_RESET "{}\n",
                    std::this_thread::get_id(),
                    m_Location.file_name(), m_Location.line(), m_Location.column(),
                    m_Location.function_name(),
                    m_Msg.empty() ? "<none>" : m_Msg
                    );

            std::println(stderr, "  " CORE_CLR_BOLD_CYAN "--- Inspected Variables ---" CORE_CLR_RESET);
            template for (const auto& [name, val] : m_Vars) {
                using CleanType = std::remove_cvref_t<decltype(val)>;
                constexpr auto type_info = std::meta::dealias(^^CleanType);
                constexpr std::string_view type_name = std::meta::display_string_of(type_info);

                std::println(stderr,
                        "    {:<20} " CORE_CLR_YELLOW "{}" CORE_CLR_RESET " " CORE_CLR_GRAY "[{:p}]" CORE_CLR_RESET " = {}",
                        type_name,
                        name,
                        static_cast<const void*>(std::addressof(val)),
                        val
                        );
            }

            std::println(stderr, "\n  " CORE_CLR_BOLD_CYAN "--- Call Stack ---" CORE_CLR_RESET);
            auto trace = std::stacktrace::current(1);
            for (std::size_t i = 0; i < trace.size(); ++i) {
                std::println(stderr, "    " CORE_CLR_GRAY "[{:>2}]" CORE_CLR_RESET " {}", i, trace[i]);
            }

            std::println(stderr,
                    "\n" CORE_CLR_BOLD_WHITE ">>> Press ENTER to resume execution..." CORE_CLR_RESET);

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

    template <detail::Printable... NewArgs>
    constexpr auto print_vars(const NewArgs&&... vars) && noexcept {
        m_Active = false;
        return breakpoint<NewArgs...>(
            m_Debug, m_Condition, m_Msg, m_Location, std::forward_as_tuple(vars...)
        );
    }
};


} // core::debug


