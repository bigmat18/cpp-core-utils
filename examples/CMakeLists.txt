cmake_minimum_required(VERSION 3.28)
project(cpp-core-utils-examples)

message(STATUS "Building examples")

file(GLOB ALL_ITEMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")

foreach (subdir ${ALL_ITEMS})
    set(SUBDIR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
    if (IS_DIRECTORY "${SUBDIR_PATH}")
        set(MAIN_FILE ${SUBDIR_PATH}/main.cpp)

        if (IS_DIRECTORY ${SUBDIR_PATH} AND EXISTS ${MAIN_FILE})
            add_executable(${subdir} ${MAIN_FILE})
            target_link_libraries(${subdir} PRIVATE cpp-core-utils)
        else()
            message(STATUS "-- main.cpp don't exists in ${subdir}, skipped")
        endif()
    endif()
endforeach()
