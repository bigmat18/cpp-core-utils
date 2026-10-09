#pragma once
#include <cstdint>
#include <stacktrace>
#include <string>
#include <expected>

#include <logging.hpp>

namespace core {

class Error {

public:
    std::string message;
    uint32_t error_code;
    std::stacktrace trace = std::stacktrace::current(1);

    Error(Error&&) noexcept = default;
    Error& operator=(Error&&) noexcept = default;
    Error(const Error&) = default;
    Error& operator=(const Error&) = default;

    [[nodiscard]] static inline auto create(std::string msg, uint32_t code) {
        return std::unexpected(Error(std::move(msg), code));
    }

private:
    Error() = delete;

    explicit Error(std::string msg, uint32_t code)
        : message(std::move(msg)), error_code(code) {}
};

template<typename T>
using Result = std::expected<T, Error>;

#define ERROR(msg, code, ...)                                                  \
    do {                                                                       \
        ::core::logging::error(                                                \
            (msg) __VA_OPT__(,) __VA_ARGS__                                    \
        );                                                                     \
        return ::core::Error::create((msg), (code));                           \
    } while (0)

#define REQUIRE(expr, msg, code, ...)                                          \
    do {                                                                       \
        if (!(expr)) [[unlikely]] {                                            \
            ERROR(msg, code __VA_OPT__(,) __VA_ARGS__);                        \
        }                                                                      \
    } while (false)

#define TRY(expr)                                                              \
    ({                                                                         \
        auto _res = (expr);                                                    \
        if (!_res.has_value()) [[unlikely]] {                                  \
            return std::unexpected(std::move(_res).error());                   \
        }                                                                      \
        std::move(_res).value();                                               \
    })

#define UNWRAP(expr)                                                           \
    ({                                                                         \
        auto _res = (expr);                                                    \
        if (!_res.has_value()) [[unlikely]] {                                  \
            const auto& _err = _res.error();                                   \
            const auto _loc = std::source_location::current();                 \
                                                                               \
            std::println(stderr,                                               \
                "\n" CORE_CLR_BOLD_RED   "[ UNWRAP FAILED ]" CORE_CLR_RESET "\n" \
                "  " CORE_CLR_BOLD_CYAN  "--- Information ---" CORE_CLR_RESET "\n" \
                "    " CORE_CLR_BOLD_WHITE "Expression : " CORE_CLR_RESET CORE_CLR_YELLOW "{}" CORE_CLR_RESET "\n" \
                "    " CORE_CLR_BOLD_WHITE "Error Code : " CORE_CLR_RESET CORE_CLR_RED "{}" CORE_CLR_RESET "\n" \
                "    " CORE_CLR_BOLD_WHITE "Location   : " CORE_CLR_RESET "{}:{}:{} in {}\n" \
                "    " CORE_CLR_BOLD_WHITE "Reason     : " CORE_CLR_RESET "{}\n\n" \
                "  " CORE_CLR_BOLD_CYAN  "--- Call Stack ---" CORE_CLR_RESET,  \
                #expr,                                                         \
                _err.error_code,                                               \
                _loc.file_name(), _loc.line(), _loc.column(),                  \
                _loc.function_name(),                                          \
                _err.message                                                   \
            );                                                                 \
                                                                               \
            for (std::size_t _i = 0; _i < _err.trace.size(); ++_i) {           \
                std::println(stderr, "    " CORE_CLR_GRAY "[{:>2}]" CORE_CLR_RESET " {}", \
                    _i, _err.trace[_i]);                                       \
            }                                                                  \
                                                                               \
            std::exit(EXIT_FAILURE);                                           \
        }                                                                      \
        std::move(_res).value();                                               \
    })
}
