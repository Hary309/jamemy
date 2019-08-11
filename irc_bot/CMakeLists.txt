cmake_minimum_required(VERSION 3.12)

project(IrcBot)

add_executable(IrcBot main.cpp)

find_library(MARIADB_LIBRARY
                libmariadb
                HINTS mariadb-connector-c/libmariadb
                PATH_SUFFIXES Debug Release
                )

target_link_libraries(IrcBot ${MARIADB_LIBRARY})
target_include_directories(IrcBot PUBLIC "mariadb-connector-c/include")
