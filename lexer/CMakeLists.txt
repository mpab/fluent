project(lexer CXX)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc /SUBSYSTEM:CONSOLE")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

cmake_minimum_required(VERSION 3.17)

set(CMAKE_SUPPRESS_REGENERATION true)

add_executable(
    lexer
    ./CMakeLists.txt
    ./src/main.cpp
    ./src/scanner.cpp
    ./src/scanner.hpp
    ./src/lexer.l
    ./src/lexer_macros.h
    ./gen/lexer.cc
    ./gen/lexer.hh)

set_property(TARGET lexer PROPERTY CXX_STANDARD 14)

target_include_directories(lexer PRIVATE ./src ./gen $ENV{FLEX_BISON_HOME})
