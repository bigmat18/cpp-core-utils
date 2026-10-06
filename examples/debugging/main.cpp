#include <mdebugging.hpp>

#include <vector>

int main() {
    int a = 5;
    float c = 10.3;
    std::vector<int> b = {10};

    core::debug::breakpoint()
        .print_vars(VAR(a), VAR(b), VAR(c))
        .when(a == 5)
        .msg("Debugging first try");

    return 0;
}
