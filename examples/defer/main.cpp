#include <defer.hpp>
#include <iostream>

int main() {
    defer(
        std::cout << "[defer] Always executed on exit.\n";
    );

    auto rollback = shdefer(
        std::cout << "[rollback] Error cleanup triggered!\n";
    );

    std::cout << "Executing operation...\n";

    rollback.dismiss();

    return 0;
}
