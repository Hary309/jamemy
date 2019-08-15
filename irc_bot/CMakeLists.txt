cmake_minimum_required(VERSION 3.12)

project(IrcBot)

set(CMAKE_CXX_STANDARD 17)

file (GLOB_RECURSE IrcBot_src
        ${CMAKE_SOURCE_DIR}/IrcBot/src/*.cpp
        ${CMAKE_SOURCE_DIR}/IrcBot/src/*.hpp)

add_executable(IrcBot ${IrcBot_src})

include(vendor/mariadb-connector-c.cmake)
include(vendor/re2.cmake)
include(vendor/loguru.cmake)

if (WIN32)
    set (EXTRA_LIBS WS2_32)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set (EXTRA_LIB pthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set (EXTRA_LIB pthread)
endif()


target_link_libraries(
    IrcBot
    loguru
    ${MARIADB_LIBRARY}
    ${RE2_LIBRARY}
    ${EXTRA_LIBS}
)

target_include_directories(IrcBot PUBLIC
                            "IrcBot/src"
                            "${CMAKE_SOURCE_DIR}/vendor/mariadb-connector-c/include"
                            "${CMAKE_SOURCE_DIR}/vendor/re2"
                            "${CMAKE_SOURCE_DIR}/vendor/loguru"
                            "${CMAKE_SOURCE_DIR}/vendor/json/single_include"
                            )
