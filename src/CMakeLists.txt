project(fluent CXX)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

cmake_minimum_required(VERSION 3.17)

find_package(BISON REQUIRED)
BISON_TARGET(parser
             parser.yy
             ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)

find_package(FLEX REQUIRED)
FLEX_TARGET(lexer
            lexer.l
            ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc)

ADD_FLEX_BISON_DEPENDENCY(lexer parser)

add_executable(fluent
    command.cpp
    console.cpp
    console.hpp
    context.cpp
    context.hpp
    logger.cpp
    logger.hpp
    main.cpp
    node.cpp
    node.hpp
    opcode_templates.hpp
    opcodes.hpp

    command.hpp
    driver.cpp
    driver.hpp
    lexeme.hpp
    lexer.l
    parser.yy
    scanner.hpp

    build_date.hpp

    ${FLEX_lexer_OUTPUTS}
    ${BISON_parser_OUTPUTS}
    ${FLEX_BISON_INTERFACE}
)

target_include_directories(fluent PRIVATE . $ENV{BOOST_HOME} $ENV{FLEX_BISON_HOME} ../build ../gen)

set_property(TARGET fluent
             PROPERTY CXX_STANDARD 14)

enable_testing()

add_test(NAME basic
         COMMAND ${CMAKE_SOURCE_DIR}/test/commands.txt
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
