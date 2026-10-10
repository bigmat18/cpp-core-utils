# C++ Core Utils Library

A lightweight, atomic, header-only collection of utility headers designed for personal modern C++ development. It serves as a foundational building block for other projects while practicing and experimenting with cutting-edge language features up to **C++26**.

- **Zero External Dependencies**: Operates strictly using the C++ standard library and GCC's experimental runtime (`libstdc++exp`), with no third-party dependencies.
- **Atomic & Zero Overhead**: Modular, self-contained utilities that impose no runtime or binary overhead when unused or disabled in release builds.
- **Nix-First Workflow**: First-class support for Nix (Flakes & Derivations), providing hermetic builds, reproducible development shells, and seamless dependency propagation into downstream CMake projects.
- **Modern C++ Practice**: Embraces modern standards and proposals (C++23/C++26), including static reflection (`<meta>`, `template for`), `std::expected`, `std::stacktrace`, and `std::print`.
- **UNIX Focused**: Tailored specifically for Linux and macOS environments (Windows compatibility is not guaranteed).

## Features

The library consists of self-contained, zero-overhead utilities located in [`core-utils/`](core-utils/):

| Header | Description | Example |
| :--- | :--- | :--- |
| [`logging.hpp`](core-utils/logging.hpp) | Zero-overhead, colored console logging with timestamps, source location, and compile-time variable reflection. | [`examples/logging/`](examples/logging/) |
| [`error.hpp`](core-utils/error.hpp) | Rust-inspired monadic error handling wrapping `std::expected` with stack traces, `TRY`, and `UNWRAP`. | [`examples/error/`](examples/error/) |
| [`defer.hpp`](core-utils/defer.hpp) | RAII scope guards implementing Go/Zig-style deferred execution with rollback support (`.dismiss()`). | [`examples/defer/`](examples/defer/) |
| [`mdebugging.hpp`](core-utils/mdebugging.hpp) | Interactive runtime breakpoints with static reflection (`<meta>`), stack traces, and variable inspection. | [`examples/debugging/`](examples/debugging/) |
| [`massert.hpp`](core-utils/massert.hpp) | Diagnostic assertions with colored terminal output, thread info, and full stack unwinding. | [`examples/asserts/`](examples/asserts/) |
| [`common.hpp`](core-utils/common.hpp) | Common concepts (`Printable`), terminal ANSI colors, and the `VAR(...)` macro for variable reflection. | — |

> For complete, runnable code examples for each module, refer to the [`examples/`](examples/) directory.

### Quick Glance

```cpp
#include <core/logging.hpp>
#include <core/error.hpp>
#include <core/defer.hpp>
#include <core/mdebugging.hpp>
#include <core/massert.hpp>

// 1. Structured Logging with Reflection (std::print + template for)
core::logging::info("Client connected", VAR(host), VAR(port));

// 2. RAII Defer with Rollback (.dismiss())
auto rollback = shdefer(std::println("Transaction failed, rolling back!"));
// ... operations ...
rollback.dismiss(); // Commit: cancel rollback

// 3. Monadic Result & Error Handling (std::expected + stacktrace)
core::Result<int> parse_port(int port) {
    REQUIRE(port > 0 && port <= 65535, "Invalid port", 1001, VAR(port));
    return port;
}
int valid_port = UNWRAP(parse_port(8080)); // Extracts value or aborts with stacktrace

// 4. Interactive Runtime Breakpoint with Type Reflection
core::breakpoint()
    .print_vars(VAR(valid_port))
    .when(valid_port == 8080)
    .msg("Inspect network configuration");

// 5. Diagnostics-Rich Assertions
massert(valid_port > 0, "Port must be positive", VAR(valid_port));
```


## Building and Integration

### Prerequisites

- **Nix** *(recommended workflow)*: Nix with Flakes enabled. Provides the compiler, tools, and dependencies automatically.
- **Manual / Standalone Toolchain** *(classic fallback)*:
  - **Compiler**: GCC 16+ supporting C++26 static reflection (`-freflection`) and `stdc++exp`.
  - **CMake**: 3.28 or newer.
  - **Build System**: [Ninja](https://ninja-build.org/) (recommended) or Make.
---

### 1. Nix (Primary Workflow)

Nix is the primary tool to develop, build, and integrate this library.

#### Development Environment
Enter a reproducible development shell with GCC 16, CMake, Ninja, and Clang tools:
```bash
nix develop
```

#### Building the Package
Build the library package using Nix:
```bash
nix build
```
This runs CMake installation under the hood, producing the header tree in `$out/include/core/` and CMake package files in `$out/lib/cmake/cpp-core-utils/`.

#### Integrating into Downstream Projects via Flakes
Add `cpp-core-utils` to your project's `flake.nix`:

```nix
{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    cpp-core-utils.url = "github:bigmat18/cpp-core-utils/V2";
  };

  outputs = { self, nixpkgs, cpp-core-utils, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      coreUtils = cpp-core-utils.packages.${system}.default;
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        packages = with pkgs; [ gcc cmake ninja ];
        # Injects core-utils into CMAKE_PREFIX_PATH automatically
        buildInputs = [ coreUtils ];
      };
    };
}
```

In your downstream `CMakeLists.txt`, locate and link the library:
```cmake
find_package(cpp-core-utils CONFIG REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE cpp-core-utils::cpp-core-utils)
```

In your C++ code:
```cpp
#include <core/logging.hpp>
```

---

### 2. CMake (Alternative / Classic Workflow)

If you are not using Nix, you can build examples and integrate the library using standard CMake.

#### Building Examples
```bash
git clone https://github.com/bigmat18/cpp-core-utils.git
cd cpp-core-utils

cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

#### Configuration Options

| Option | Default | Description |
| :--- | :--- | :--- |
| `CORE_UTILS_BUILD_EXAMPLES` | `ON` | Build example executables in [`examples/`](examples/). |
| `CORE_UTILS_INCLUDE_NAME` | `"core"` | Subdirectory name where headers are installed (e.g. `<core/...>`). |
| `CORE_UTILS_CUSTOM_INSTALL_DIR` | `""` | Optional direct destination directory to copy header files during install. |

To configure without building examples:
```bash
cmake -B build -G Ninja -DCORE_UTILS_BUILD_EXAMPLES=OFF
```

#### Installation and `find_package`
Install the headers and CMake configuration files into a system directory or a custom prefix:

```bash
# System install
cmake --install build --prefix /usr/local

# Or install to any custom prefix directory
cmake --install build --prefix /path/to/install/dir
```

In your downstream project's `CMakeLists.txt`:
```cmake
# If using a custom install prefix, add it to CMAKE_PREFIX_PATH:
# list(APPEND CMAKE_PREFIX_PATH "/path/to/install/dir")

find_package(cpp-core-utils CONFIG REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE cpp-core-utils::cpp-core-utils)
```

#### Direct Inclusion via `FetchContent`
You can also embed the repository directly into your CMake project without prior installation:

```cmake
include(FetchContent)
FetchContent_Declare(
    cpp-core-utils
    GIT_REPOSITORY https://github.com/bigmat18/cpp-core-utils.git
    GIT_TAG        V2
)
set(CORE_UTILS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cpp-core-utils)

target_link_libraries(my_app PRIVATE cpp-core-utils)
```