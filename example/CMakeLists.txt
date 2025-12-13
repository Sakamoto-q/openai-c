cmake_minimum_required(VERSION 3.15)
project(tester C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(cJSON CONFIG REQUIRED)
find_package(openai-c REQUIRED)

add_executable(test src/main.c)

target_link_libraries(test PRIVATE openai-c::openai-c CURL::libcurl cjson)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)