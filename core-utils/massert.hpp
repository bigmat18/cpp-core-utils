#pragma once

#include <cstdlib>
#include <format>
#include <source_location>
#include <string>
#include <string_view>
#include <print>
#include <thread>
#include <stacktrace>

#include <common.hpp>

namespace core {

namespace detail {

template<Printable...Args>
static inline void assert(
    const bool condition,
    const std::string_view expr,
    const std::string_view message,
    const std::source_location& location,
    Args&& ...args) 
{
    if (!condition) [[unlikely]] {
        std::string formatted_buffer;
        std::string_view final_msg = message;

        if constexpr (sizeof...(Args) > 0) {
            formatted_buffer = std::vformat(message, std::make_format_args(args...));
            final_msg = formatted_buffer;
        }

        std::println(stderr,
            "\n" CORE_CLR_BOLD_RED   "[ ASSERTION FAILED ]" CORE_CLR_RESET "\n"
            "  " CORE_CLR_BOLD_CYAN  "--- Information ---" CORE_CLR_RESET "\n"
            "    " CORE_CLR_BOLD_WHITE "Condition : " CORE_CLR_RESET CORE_CLR_YELLOW "{}" CORE_CLR_RESET "\n"
            "    " CORE_CLR_BOLD_WHITE "Thread ID : " CORE_CLR_RESET "{}\n"
            "    " CORE_CLR_BOLD_WHITE "Location  : " CORE_CLR_RESET "{}:{}:{} in {}\n"
            "    " CORE_CLR_BOLD_WHITE "Message   : " CORE_CLR_RESET "{}\n\n"
            "  " CORE_CLR_BOLD_CYAN  "--- Call Stack ---" CORE_CLR_RESET,
            expr,
            std::this_thread::get_id(),
            location.file_name(), location.line(), location.column(),
            location.function_name(),
            final_msg
        );

        auto trace = std::stacktrace::current(1);
        for (std::size_t i = 0; i < trace.size(); ++i) {
            std::println(stderr, "    " CORE_CLR_GRAY "[{:>2}]" CORE_CLR_RESET " {}", i, trace[i]);
        }

        std::exit(EXIT_FAILURE);
    }
}

}

#ifndef NDEBUG

#define massert(condition, message, ...)                                       \
    ::core::detail::assert(                                               \
        static_cast<bool>(condition),                                          \
        #condition,                                                            \
        (message),                                                             \
        std::source_location::current()                                        \
        __VA_OPT__(,) __VA_ARGS__                                              \
    )

#else

#define massert(condition, message, ...) ((void)0)

#endif // !NDEBUG


}
