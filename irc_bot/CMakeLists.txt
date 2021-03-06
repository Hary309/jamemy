cmake_minimum_required(VERSION 3.12)

project(IrcBot)

set(CMAKE_CXX_STANDARD 17)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file (GLOB_RECURSE IrcBot_src
        ${CMAKE_SOURCE_DIR}/IrcBot/src/*.cpp
        ${CMAKE_SOURCE_DIR}/IrcBot/src/*.hpp)

add_executable(IrcBot ${IrcBot_src})

include(vendor/mariadb-connector-c.cmake)
include(vendor/re2.cmake)
include(vendor/loguru.cmake)
include(vendor/libircclient.cmake)
include(vendor/smtp-client.cmake)

set (EXTRA_LIBS "")

if (WIN32)
    string(CONCAT EXTRA_LIBS WS2_32)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    string(CONCAT EXTRA_LIBS pthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    string(CONCAT EXTRA_LIBS pthread)
endif()


target_link_libraries(
    IrcBot
    loguru
    libircclient
    smtp-client
    ${MARIADB_LIBRARY}
    ${RE2_LIBRARY}
    ${EXTRA_LIBS}
)

target_include_directories(IrcBot PUBLIC
                            "${CMAKE_SOURCE_DIR}/IrcBot/src"
                            "${CMAKE_SOURCE_DIR}/vendor/mariadb-connector-c/include"
                            "${CMAKE_SOURCE_DIR}/vendor/mariadb-connector-c/bin/include"
                            "${CMAKE_SOURCE_DIR}/vendor/re2"
                            "${CMAKE_SOURCE_DIR}/vendor/loguru"
                            "${CMAKE_SOURCE_DIR}/vendor/json/single_include"
                            "${CMAKE_SOURCE_DIR}/vendor/libircclient/include"
                            "${CMAKE_SOURCE_DIR}/vendor/smtp-client/src"
                            "${CMAKE_SOURCE_DIR}/vendor/HTTPRequest/include"
                            )
