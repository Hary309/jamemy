cmake_minimum_required(VERSION 3.12)

project(IrcBot)

file (GLOB_RECURSE IrcBot_src
        src/*.cpp
        src/*.hpp)

add_executable(IrcBot ${IrcBot_src})

find_library(MARIADB_LIBRARY
                NAMES libmariadb mariadb
                HINTS ${CMAKE_SOURCE_DIR}/vendor/mariadb-connector-c/libmariadb
                PATH_SUFFIXES . Debug Release
                )

target_link_libraries(IrcBot ${MARIADB_LIBRARY})
target_include_directories(IrcBot PUBLIC "${CMAKE_SOURCE_DIR}/vendor/mariadb-connector-c/include")
