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

option(CORE_UTILS_BUILD_EXAMPLES "Build examples" ON)

set(CORE_UTILS_INCLUDE_NAME "core" CACHE STRING "Subdirectory name for installed headers")
set(CORE_UTILS_CUSTOM_INSTALL_DIR "" CACHE PATH "Optional custom directory to directly copy header files during install")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/include/${CORE_UTILS_INCLUDE_NAME})
    file(CREATE_LINK ${UTILS_DIR} ${CMAKE_CURRENT_BINARY_DIR}/include/${CORE_UTILS_INCLUDE_NAME} SYMBOLIC)
endif()

add_library(cpp-core-utils INTERFACE)
target_include_directories(cpp-core-utils INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${UTILS_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${CORE_UTILS_INCLUDE_NAME}>
)

target_compile_options(cpp-core-utils INTERFACE
     $<$<CXX_COMPILER_ID:GNU>:-freflection>
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(cpp-core-utils INTERFACE stdc++exp)
endif()

install(TARGETS cpp-core-utils
    EXPORT cpp-core-utilsTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} ${CMAKE_INSTALL_INCLUDEDIR}/${CORE_UTILS_INCLUDE_NAME}
)

install(DIRECTORY ${UTILS_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CORE_UTILS_INCLUDE_NAME}
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

if(CORE_UTILS_CUSTOM_INSTALL_DIR)
    install(DIRECTORY ${UTILS_DIR}/
        DESTINATION ${CORE_UTILS_CUSTOM_INSTALL_DIR}
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
    )
endif()

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

if(CORE_UTILS_BUILD_EXAMPLES)
    add_subdirectory(${EXAMPLES_DIR})
endif()
