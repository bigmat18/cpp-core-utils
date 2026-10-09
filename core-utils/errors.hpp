#pragma once
#include <cstdint>
#include <stacktrace>
#include <string>
#include <expected>
#include <logging.hpp>

namespace core::errors {

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
        return ::core::errors::Error::create((msg), (code));                   \
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
                "\n\033[1;31m[ UNWRAP FAILED ]\033[0m\n"                       \
                "\033[1;37mExpression :\033[0m \033[33m{}\033[0m\n"            \
                "\033[1;37mError Code :\033[0m \033[31m{}\033[0m\n"            \
                "\033[1;37mReason     :\033[0m {}\n"                           \
                "\033[1;34m--- Call Stack ---\033[0m",                         \
                #expr,                                                         \
                _err.error_code,                                               \
                _err.message                                                   \
            );                                                                 \
                                                                               \
            for (std::size_t _i = 0; _i < _err.trace.size(); ++_i) {           \
                std::println(stderr, "  \033[90m[{:>2}]\033[0m {}",            \
                    _i, _err.trace[_i]);                                       \
            }                                                                  \
                                                                               \
            std::exit(EXIT_FAILURE);                                           \
        }                                                                      \
        std::move(_res).value();                                               \
    })
}
