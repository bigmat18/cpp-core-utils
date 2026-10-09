#pragma once

#include <print>
#include <meta>
#include <source_location>
#include <string_view>
#include <chrono>

#include <common.hpp>

namespace core::logging {

namespace detail {

constexpr std::string_view extract_file_name(std::string_view path) noexcept {
    const auto pos = path.find_last_of("/\\");
    return (pos == std::string_view::npos) ? path : path.substr(pos + 1);
}

template<core::detail::Printable...Args>
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
        std::println("{}[{}][{}][{}:{}:{}] {}" CORE_CLR_RESET, 
                color, level, timestamp_buffer, file, location.line(), 
                location.column(), message);
    } else {
        std::string vars_str = "";
        template for (const auto&& [name, val] : std::forward_as_tuple(std::forward<Args>(args)...)) {
            vars_str += std::format("{}={} ", name, val);
        }
        std::println("{}[{}][{}][{}:{}:{}] {} | {}" CORE_CLR_RESET, 
                color, level, timestamp_buffer, file, location.line(), 
                location.column(), message, vars_str);
    }
}

} // detail


template<core::detail::Printable...Args>
struct error {
    inline error(const std::string_view message, Args&&...args,
                 const std::source_location location = std::source_location::current()) {
        detail::internal("ERROR", CORE_CLR_RED, message, location, std::forward<Args>(args)...);
    }
};

template<core::detail::Printable... Args>
error(std::string_view, Args&&...) -> error<Args...>;


template<core::detail::Printable...Args>
struct info {
    inline info(const std::string_view message, Args&&...args,
                   const std::source_location location = std::source_location::current()) {
        detail::internal("INFO", CORE_CLR_GREEN, message, location, std::forward<Args>(args)...);
    }
};

template<core::detail::Printable... Args>
info(std::string_view, Args&&...) -> info<Args...>;

template<core::detail::Printable...Args>
struct warn {
    inline warn(const std::string_view message, Args&&...args,
                   const std::source_location location = std::source_location::current()) {
        detail::internal("WARN", CORE_CLR_YELLOW, message, location, std::forward<Args>(args)...);
    }
};

template<core::detail::Printable... Args>
warn(std::string_view, Args&&...) -> warn<Args...>;

template<core::detail::Printable...Args>
struct debug {
    inline debug(const std::string_view message, Args&&...args,
                    const std::source_location location = std::source_location::current()) {
#ifndef NDEBUG
        detail::internal("DEBUG", CORE_CLR_CYAN, message, location, std::forward<Args>(args)...);
#endif // NDEBUG
    }
};

template<core::detail::Printable... Args>
debug(std::string_view, Args&&...) -> debug<Args...>;


} // core::logging
