cmake_minimum_required(VERSION 3.15)
project(openai-c C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /Zi")
    add_compile_options(/utf-8)
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(CURL REQUIRED)
find_package(cJSON CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.c")
option(BUILD_SHARED_LIBS "Build shared library instead of static" OFF)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC
        CURL::libcurl 
        cjson
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 crypt32 wldap32)
endif()

file(GLOB HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")
install(
    FILES ${HEADERS}
    DESTINATION include/${PROJECT_NAME}
)

install(TARGETS ${PROJECT_NAME}
    EXPORT openai-c-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/openai-cConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/openai-cConfig.cmake"
    INSTALL_DESTINATION share/openai-c/cmake
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/openai-cConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
)

install(EXPORT openai-c-targets
    FILE openai-cTargets.cmake
    NAMESPACE openai-c::
    DESTINATION share/openai-c/cmake
)

install(
    FILES 
        "${CMAKE_CURRENT_BINARY_DIR}/openai-cConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/openai-cConfigVersion.cmake"
    DESTINATION share/openai-c/cmake
)