find_library(MARIADB_LIBRARY
                NAMES libmariadb mariadb
                HINTS ${CMAKE_SOURCE_DIR}/vendor/mariadb-connector-c/libmariadb
                PATH_SUFFIXES . Debug Release
                )
