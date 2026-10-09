#pragma once

#include <format>
#include <print>
#include <meta>
#include <source_location>
#include <string_view>
#include <chrono>
#include <utility>

namespace core::logging {

#if !defined (_WIN32)
    #define RED     "\x1b[31m"
    #define YELLOW  "\x1b[33m"
    #define GREEN   "\x1b[32m"
    #define BLUE    "\x1b[36m"
    #define RESET   "\x1b[0m"
#else
    #define RED     ""
    #define YELLOW  ""
    #define GREEN   ""
    #define BLUE    ""
    #define RESET   ""
#endif

namespace detail {

template<typename T>
concept Loggable = requires(T val)
{
    val.first;
    val.second;
} && std::convertible_to<decltype(std::declval<T>().first), std::string_view>
  && std::formattable<std::remove_cvref_t<decltype(std::declval<T>().second)>, char>;

constexpr std::string_view extract_file_name(std::string_view path) noexcept {
    const auto pos = path.find_last_of("/\\");
    return (pos == std::string_view::npos) ? path : path.substr(pos + 1);
}

template<detail::Loggable...Args>
inline void internal(const std::string_view level, 
        const std::string_view color, 
        const std::string_view message, 
        const std::source_location& location,
        Args&&... args)
{
    std::string_view file = extract_file_name(location.file_name());
    auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char timestamp_buffer[64];
    std::strftime(timestamp_buffer, sizeof(timestamp_buffer),
            "%Y-%m-%d %X", std::localtime(&in_time_t));

    if constexpr (sizeof...(Args) == 0) {
        std::println("{}[{}][{}][{}:{}:{}] {}{}", 
                color, level, timestamp_buffer, file, location.line(), 
                location.column(), message, RESET);
    } else {
        std::string vars_str = "";
        template for (const auto&& [name, val] : std::forward_as_tuple(std::forward<Args>(args)...)) {
            vars_str += std::format("{}={} ", name, val);
        }
        std::println("{}[{}][{}][{}:{}:{}] {} | {}{}", 
                color, level, timestamp_buffer, file, location.line(), 
                location.column(), message, vars_str, RESET);
    }
}

} // detail


template<detail::Loggable...Args>
struct error {
    inline error(const std::string_view message, Args&&...args,
                 const std::source_location location = std::source_location::current()) {
        detail::internal("ERROR", RED, message, location, std::forward<Args>(args)...);
    }
};

template<detail::Loggable... Args>
error(std::string_view, Args&&...) -> error<Args...>;


template<detail::Loggable...Args>
struct info {
    inline info(const std::string_view message, Args&&...args,
                   const std::source_location location = std::source_location::current()) {
        detail::internal("INFO", GREEN, message, location, std::forward<Args>(args)...);
    }
};

template<detail::Loggable... Args>
info(std::string_view, Args&&...) -> info<Args...>;

template<detail::Loggable...Args>
struct warn {
    inline warn(const std::string_view message, Args&&...args,
                   const std::source_location location = std::source_location::current()) {
        detail::internal("WARN", YELLOW, message, location, std::forward<Args>(args)...);
    }
};

template<detail::Loggable... Args>
warn(std::string_view, Args&&...) -> warn<Args...>;

template<detail::Loggable...Args>
struct debug {
    inline debug(const std::string_view message, Args&&...args,
                    const std::source_location location = std::source_location::current()) {
#ifndef NDEBUG
        detail::internal("DEBUG", BLUE, message, location, std::forward<Args>(args)...);
#endif // NDEBUG
    }
};

template<detail::Loggable... Args>
debug(std::string_view, Args&&...) -> debug<Args...>;


#define VAR(var) (std::pair<std::string_view, const decltype(var)&>{ #var, (var) })

} // core::logging
