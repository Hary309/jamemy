add_library(
    loguru STATIC
    ${CMAKE_SOURCE_DIR}/vendor/loguru/loguru.cpp
    ${CMAKE_SOURCE_DIR}/vendor/loguru/loguru.hpp
)

add_compile_definitions(loguru PUBLIC LOGURU_STACKTRACES=0)
