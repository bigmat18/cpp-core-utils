#include <mdebugging.hpp>

#include <vector>

int main() {
    int a = 5;
    float c = 10.3;
    std::vector<int> b = {10};

    core::breakpoint()
        .print_vars(VAR(a), VAR(b), VAR(c))
        .when(a == 5)
        .msg("Debugging in release");

    core::breakpoint()
        .print_vars(VAR(a), VAR(b), VAR(c))
        .when(a == 5)
        .msg("Debugging in debug")
        .if_debug();

    return 0;
}
