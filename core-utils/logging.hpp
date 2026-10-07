#pragma once

#include <format>
#include <print>
#include <source_location>
#include <string_view>
#include <chrono>

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

inline void internal(const std::string_view level, 
        const std::string_view color, 
        const std::string_view message, 
        const std::source_location& location,
        std::format_args args)
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    char timestamp_buffer[64];
    std::strftime(timestamp_buffer, sizeof(timestamp_buffer),
            "%Y-%m-%d %X", std::localtime(&in_time_t));

    std::string full_message = std::vformat(message, args);
    std::println("{}[{}][{}:{}:{}][{}] {}{}", 
            color, timestamp_buffer, location.file_name(), location.line(), 
            location.column(), level, full_message, RESET);
}

} // detail


template<typename...Args>
struct error {
    inline error(const std::string_view message, Args&&...args,
                 const std::source_location location = std::source_location::current()) {
        detail::internal("ERROR", RED, message, location, std::make_format_args(args...));
    }
};

template<typename... Args>
error(std::string_view, Args&&...) -> error<Args...>;

template<typename...Args>
struct info {
    inline info(const std::string_view message, Args&&...args,
                   const std::source_location location = std::source_location::current()) {
        detail::internal("INFO", GREEN, message, location, std::make_format_args(args...));
    }
};

template<typename... Args>
info(std::string_view, Args&&...) -> info<Args...>;

template<typename...Args>
struct warn {
    inline warn(const std::string_view message, Args&&...args,
                   const std::source_location location = std::source_location::current()) {
        detail::internal("WARN", YELLOW, message, location, std::make_format_args(args...));
    }
};

template<typename... Args>
warn(std::string_view, Args&&...) -> warn<Args...>;

template<typename...Args>
struct debug {
    inline debug(const std::string_view message, Args&&...args,
                    const std::source_location location = std::source_location::current()) {
        detail::internal("DEBUG", BLUE, message, location, std::make_format_args(args...));
    }
};

template<typename... Args>
debug(std::string_view, Args&&...) -> debug<Args...>;

} // core::logging
