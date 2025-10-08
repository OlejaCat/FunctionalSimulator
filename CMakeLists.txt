cmake_minimum_required(VERSION 3.14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(simulator) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_DEBUG "Enable debug build" OFF)
option(ENABLE_CLANG_TIDY "Enable clang-tidy checks" OFF)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY clang-tidy)
    if(CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY 
            ${CLANG_TIDY}
        )
        message(STATUS "Clang-tidy enabled: ${CLANG_TIDY}")
    else()
        message(WARNING "Clang-tidy not found!")
    endif()
endif()

include(cmake/compiler_flags.cmake)

add_executable(simulator)

target_include_directories(simulator
    PRIVATE
        include/
)

target_sources(simulator
    PRIVATE
        src/main.cpp
        src/memory.cpp
        src/simulator.cpp
)

target_link_libraries(simulator project_compiler_flags)
