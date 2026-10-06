cmake_minimum_required(VERSION 3.28)
project(cpp-core-utils LANGUAGES CXX VERSION 2.0.0)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/core-utils)

option(CPPUTILS_BUILD_EXAMPLES "Build examples" ON)
option(CPPUTILS_ENABLE_PROFILING "Profiling utils" ON)
option(CPPUTILS_ENABLE_ASSERT "Enable assertions" ON)
option(CPPUTILS_ENABLE_DEBUG "Enable debug utilities" ON)
option(CPPUTILS_ENABLE_LOGGING "Enable logging" ON)

add_library(cpp-core-utils INTERFACE)
target_include_directories(cpp-core-utils INTERFACE 
    $<BUILD_INTERFACE:${UTILS_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_options(cpp-core-utils INTERFACE
     $<$<CXX_COMPILER_ID:GNU>:-freflection>
)

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
