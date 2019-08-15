find_library(RE2_LIBRARY
                NAMES re2 libre2
                HINTS ${CMAKE_SOURCE_DIR}/vendor/re2/bin
                PATH_SUFFIXES . Debug Release)