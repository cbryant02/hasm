cmake_minimum_required(VERSION 3.10)
project(hasm)

# Set standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Use ccache
find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif ()

# Add include dir and source files
include_directories(${CMAKE_SOURCE_DIR}/src/include)
add_executable(hasm
        src/include/assembly_parser.hh
        src/include/common.hh
        src/include/instruction.hh
        src/include/symbols.hh
        src/ainstruction.cc
        src/assembly_parser.cc
        src/cinstruction.cc
        src/instruction.cc
        src/main.cc
        src/symbols.cc)