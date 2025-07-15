cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_COMPILER "g++")

set(ENV{CXX} "/usr/bin/g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "C++ standard ${CMAKE_CXX_STANDARD}")

project(Stack)


set(COMMON_FLAGS "-mrdrnd -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_FLAGS} -Og -DDEBUG -g -fsanitize=address -D_FORTIFY_SOURCE=2")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_FLAGS} -O2 -DNDEBUG -DNCANARIES_MODE -DNHASH_MODE")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Flags used for compiling: ${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Flags used for compiling: ${CMAKE_CXX_FLAGS_RELEASE}")
else()
    message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()


include_directories(include)

set(SOURCE_DIR source)
set(SOURCE ${SOURCE_DIR}/stack_utils.cpp ${SOURCE_DIR}/stack.cpp)

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
add_library(stack STATIC ${SOURCE})
