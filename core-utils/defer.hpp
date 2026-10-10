#pragma once

#include <functional>
#include <utility>

namespace core {

class Defer {
    using TFun = std::move_only_function<void()>;

    bool m_Active = true;
    TFun m_Fun;
public:
    explicit Defer(TFun fun) : m_Fun(std::move(fun)) {}
    ~Defer() { if(m_Active) m_Fun(); }

    Defer(const Defer&) = delete;
    Defer& operator=(const Defer&) = delete;

    void dismiss() noexcept { m_Active = false; }
};

#define DEFER_CONCAT(x, y) x##y

// classic defer
#define defer(...) auto DEFER_CONCAT(_defer_guard_, __LINE__) = ::core::Defer([&]() {__VA_ARGS__})

// self-handled defer
#define shdefer(...) ::core::Defer([&]() {__VA_ARGS__})

} // core
