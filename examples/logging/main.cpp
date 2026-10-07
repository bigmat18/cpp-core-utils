#include <logging.hpp>
#include <print>

int main(void) {
    int a = 5;

    core::logging::error("Error in {}", a);
    core::logging::info("Info in {}", a);
    core::logging::warn("Warn in {}", a);
    core::logging::debug("Debug in {}", a);

    std::println("Normal message");

    return 0;
}
