cmake_minimum_required(VERSION 3.12)

project(IrcBot)

set(CMAKE_CXX_STANDARD 17)

file (GLOB_RECURSE IrcBot_src
        ${CMAKE_SOURCE_DIR}/IrcBot/src/*.cpp
        ${CMAKE_SOURCE_DIR}/IrcBot/src/*.hpp)

add_executable(IrcBot ${IrcBot_src})

find_library(MARIADB_LIBRARY
                NAMES libmariadb mariadb
                HINTS ${CMAKE_SOURCE_DIR}/vendor/mariadb-connector-c/libmariadb
                PATH_SUFFIXES . Debug Release
                )

if (WIN32)
    set (EXTRA_LIBS WS2_32)
endif()

target_link_libraries(IrcBot ${MARIADB_LIBRARY} ${EXTRA_LIBS})
target_include_directories(IrcBot PUBLIC "IrcBot/src" "${CMAKE_SOURCE_DIR}/vendor/mariadb-connector-c/include")
