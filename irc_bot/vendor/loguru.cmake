add_library(
    loguru STATIC
    ${CMAKE_SOURCE_DIR}/vendor/loguru/loguru.cpp
    ${CMAKE_SOURCE_DIR}/vendor/loguru/loguru.hpp
)

target_compile_definitions(loguru PRIVATE LOGURU_STACKTRACES=0)
