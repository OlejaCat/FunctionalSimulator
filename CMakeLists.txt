cmake_minimum_required(VERSION 3.14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(simulator) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_DEBUG "Enable debug build" OFF)
option(ENABLE_CLANG_TIDY "Enable clang-tidy checks" OFF)
option(BUILD_TESTS "Enable building tests" OFF)

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
        src/cpu.cpp
        src/memory.cpp
        src/simulator.cpp
        src/instruction_parser.cpp
)

target_link_libraries(simulator project_compiler_flags)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL "https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip"
    )
    
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    add_executable(tests 
                      tests/instruction_parser_tests.cpp 
                      src/instruction_parser.cpp
                      tests/memory_tests.cpp
                      src/memory.cpp)
    target_include_directories(tests PRIVATE include/)
    if (ENABLE_DEBUG)
        target_link_libraries(tests GTest::gtest_main)
    else()
        target_link_libraries(tests GTest::gtest_main project_compiler_flags)
    endif()

    include(GoogleTest)
    gtest_discover_tests(tests)
endif()
