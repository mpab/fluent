cmake_minimum_required(VERSION 3.17)

# generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# pass in using cmake -DTARGET_NAME:STRING="target_name"
SET(TARGET_NAME "target_name" CACHE STRING "target name")
message(STATUS "Using TARGET_NAME=${TARGET_NAME}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")

# set the project name and version
project(${TARGET_NAME} CXX)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

#set(CMAKE_FIND_DEBUG_MODE TRUE)
find_package(BISON REQUIRED)
#set(CMAKE_FIND_DEBUG_MODE FALSE)
BISON_TARGET(${TARGET_NAME}
             ./src/parser.y
             ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cc
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.hh)

find_package(FLEX REQUIRED)
FLEX_TARGET(${TARGET_NAME}
            ./src/lexer.l
            ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc)

ADD_FLEX_BISON_DEPENDENCY(${TARGET_NAME} ${TARGET_NAME})

execute_process (
    COMMAND bash -c "printf 'const char build_date[] = \"%s\";\n' '$(date)'"
    OUTPUT_VARIABLE outVar
)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/build_date.h" "${outVar}")

add_executable(
    ${TARGET_NAME}
    ./src/${TARGET_NAME}.cpp
    ./src/console.cpp
    ./src/context.cpp
    ./src/logger.cpp
    ./src/node.cpp
    ./src/opcodes.cpp
    ./src/opcode_map.cpp
    ${FLEX_${TARGET_NAME}_OUTPUTS}
    ${BISON_${TARGET_NAME}_OUTPUTS}
)

set_property(
    TARGET ${TARGET_NAME}
    PROPERTY CXX_STANDARD 20
)

target_include_directories(
    ${TARGET_NAME} PRIVATE ./src ${CMAKE_CURRENT_BINARY_DIR} $ENV{FLEX_INCLUDE_DIR} $ENV{BOOST_HOME} $ENV{INC_EXT}
)
