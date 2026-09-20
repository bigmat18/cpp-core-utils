# C++ Utils Library

A lightweight, header-only collection of utility headers for C++ development. These utilities simplify debugging, profiling, logging, assertions, and formatted output for common data structures. The library is designed for easy integration into modern C++ projects (C++23+).

## Features

The utilities are provided in the `utils/` directory as individual header files:

- **`debug.hpp`**: Runtime debugging tools, including breakpoints for pausing execution and inspecting state.
- **`profiling.hpp`**: Profiling utilities that measure delta times for code scopes, generating hierarchical trees of execution timings for performance analysis.
- **`logging.hpp`**: Flexible logging system with support for console output and file persistence, including severity levels and timestamps.
- **`massert.hpp`**: Custom assertion macros enhanced with stack traces for better error diagnosis and debugging.
- **`formatting.hpp`**: Enhanced printing utilities for `std::print` and `std::format`, supporting vectors, maps, tuples, and other basic C++ containers with customizable output formatting.

## Building and Installation

This is a header-only library, so no compilation is required for the core utilities. CMake is used for managing dependencies, building examples, or running tests.

1. Clone the repository:
```
git clone https://github.com/bigmat18/cpp-core-utils.git
cd cpp-core-utils
```
2. Create a build directory:
```
mkdir build
```
3. Configure and build with CMake:
```
cmake -G "Ninja" . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --verbose
```

For a simple integration into your project:
- Add the `utils/` directory to your include path.
- Include headers directly (e.g., `#include <utils/debug.hpp>`).
- No linking is needed since it's header-only.

### Prerequisites
- CMake 3.24+
- C++23 compliant compiler (e.g., GCC 13+, Clang 15+, MSVC 2022+)

### CMake Configuration Options

The library supports several CMake options to enable or disable features and build examples. These can be set during configuration (e.g., via `-DBUILD_EXAMPLES=OFF`) or through your CMake GUI/IDE.

- **`BUILD_EXAMPLES`** (default: `ON`): Enables building of example executables in the `examples/` directory.
- **`USE_OPENMP`** (default: `OFF`): Activates OpenMP support in the utilities for parallel processing. Requires OpenMP to be installed and detected.
- **`ENABLE_ASSERT`** (default: `ON`): Enables custom assertion utilities (from `massert.hpp`).
- **`ENABLE_DEBUG`** (default: `ON`): Enables debugging utilities (from `debug.hpp`).
- **`ENABLE_PROFILING`** (default: `ON`): Enables profiling utilities (from `profiling.hpp`).
- **`ENABLE_LOGGING`** (default: `ON`): Enables logging utilities (from `logging.hpp`).

Disabling these options removes the corresponding compile-time definitions (`ENABLE_ASSERT`, `ENABLE_DEBUG`, etc.) to reduce overhead in production builds.

## Usage

Examples demonstrating the usage of each utility are provided in the `examples/` directory. Build them by enabling `BUILD_EXAMPLES=ON` (default) during CMake configuration.
Refer to each header's inline documentation for detailed API usage, configuration options, and further details.
