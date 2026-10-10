#pragma once

#include <utility>
#include <format>

namespace core::detail {

template<typename T>
concept Printable = requires(T val)
{
    val.first;
    val.second;
} && std::convertible_to<decltype(std::declval<T>().first), std::string_view>
  && std::formattable<std::remove_cvref_t<decltype(std::declval<T>().second)>, char>;

#define VAR(var) (std::pair<std::string_view, const decltype(var)&>{ #var, (var) })

#if !defined(_WIN32)
    #define CORE_CLR_RESET       "\x1b[0m"
    #define CORE_CLR_BOLD        "\x1b[1m"
    #define CORE_CLR_GRAY        "\x1b[90m"

    #define CORE_CLR_RED         "\x1b[31m"
    #define CORE_CLR_YELLOW      "\x1b[33m"
    #define CORE_CLR_GREEN       "\x1b[32m"
    #define CORE_CLR_CYAN        "\x1b[36m"
    #define CORE_CLR_BLUE        "\x1b[34m"

    #define CORE_CLR_BOLD_RED    "\x1b[1;31m"
    #define CORE_CLR_BOLD_YELLOW "\x1b[1;33m"
    #define CORE_CLR_BOLD_WHITE  "\x1b[1;37m"
    #define CORE_CLR_BOLD_CYAN   "\x1b[1;36m"
#else
    #define CORE_CLR_RESET       ""
    #define CORE_CLR_BOLD        ""
    #define CORE_CLR_GRAY        ""
    #define CORE_CLR_RED         ""
    #define CORE_CLR_YELLOW      ""
    #define CORE_CLR_GREEN       ""
    #define CORE_CLR_CYAN        ""
    #define CORE_CLR_BLUE        ""
    #define CORE_CLR_BOLD_RED    ""
    #define CORE_CLR_BOLD_YELLOW ""
    #define CORE_CLR_BOLD_WHITE  ""
    #define CORE_CLR_BOLD_CYAN   ""
#endif

} // core::detail

