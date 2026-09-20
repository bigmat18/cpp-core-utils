cmake_minimum_required(VERSION 3.10)
project(cpp-core-utils LANGUAGES CXX VERSION 1.0.0)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils)

option(CPPUTILS_BUILD_EXAMPLES "Build examples" ON)
option(CPPUTILS_USE_OPENMP "Enable OpenMP" OFF)
option(CPPUTILS_ENABLE_WARNINGS "Enable recommended warnings" OFF)
option(CPPUTILS_ENABLE_PROFILING "Profiling utils" ON)
option(CPPUTILS_ENABLE_ASSERT "Enable assertions" ON)
option(CPPUTILS_ENABLE_DEBUG "Enable debug utilities" ON)
option(CPPUTILS_ENABLE_LOGGING "Enable logging" ON)

add_library(cpp-core-utils INTERFACE)
target_include_directories(cpp-core-utils INTERFACE 
    $<BUILD_INTERFACE:${UTILS_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(CPPUTILS_USE_OPENMP)
    find_package(OpenMP QUIET)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(cpp-core-utils INTERFACE OpenMP::OpenMP_CXX)
        target_compile_definitions(cpp-core-utils INTERFACE USE_OPENMP)
    endif()
endif()

if(CPPUTILS_ENABLE_WARNINGS)
    target_compile_options(cpp-core-utils INTERFACE 
        -Wall 
        -Wextra 
        -Wpedantic 
        -Wcpp 
        $<$<CXX_COMPILER_ID:Clang>:-Weverything -Wno-c++98-compat>
        $<$<CXX_COMPILER_ID:GNU>:-Wshadow=local -Wformat=2>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4996>
    )
endif()

if(CPPUTILS_ENABLE_ASSERT)
    target_compile_definitions(cpp-core-utils INTERFACE ENABLE_ASSERT)
endif()

if(CPPUTILS_ENABLE_DEBUG)
    target_compile_definitions(cpp-core-utils INTERFACE ENABLE_DEBUG)
endif()

if(CPPUTILS_ENABLE_LOGGING)
    target_compile_definitions(cpp-core-utils INTERFACE ENABLE_LOGGING)
endif()

if(CPPUTILS_ENABLE_PROFILING)
    target_compile_definitions(cpp-core-utils INTERFACE ENABLE_PROFILING)
endif()

install(TARGETS cpp-core-utils
    EXPORT cpp-core-utilsTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${UTILS_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/utils
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

install(EXPORT cpp-core-utilsTargets
    FILE cpp-core-utilsTargets.cmake
    NAMESPACE cpp-core-utils::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp-core-utils
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/cpp-core-utilsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/cpp-core-utilsConfig.cmake"
    "include(CMakeFindDependencyMacro)
    include(\"\${CMAKE_CURRENT_LIST_DIR}/cpp-core-utilsTargets.cmake\")"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cpp-core-utilsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cpp-core-utilsConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp-core-utils
)

if(CPPUTILS_BUILD_EXAMPLES)
    add_subdirectory(${EXAMPLES_DIR})
endif()
