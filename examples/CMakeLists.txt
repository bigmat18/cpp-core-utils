cmake_minimum_required(VERSION 3.24)
project(cpp-core-utils-examples)

message(STATUS "Building examples")
set(EXAMPLES 
    01-assert
    02-logging
    03-profiling
    04-debug
)

foreach (subdir ${EXAMPLES})
    set(SUBDIR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
    set(MAIN_FILE ${SUBDIR_PATH}/main.cpp)

    if (IS_DIRECTORY ${SUBDIR_PATH} AND EXISTS ${MAIN_FILE})

        add_executable(${subdir} ${MAIN_FILE})
        target_link_libraries(${subdir} PRIVATE cpp-core-utils)

        if(USE_OPENMP AND TARGET OpenMP::OpenMP_CXX)
            target_link_libraries(${subdir} PRIVATE OpenMP::OpenMP_CXX)
        endif()


        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_link_libraries(${subdir} PRIVATE stdc++exp)
        endif()

    else()
        message(STATUS "-- main.cpp don't exists in ${subdir}, skipped")
    endif()
endforeach()
