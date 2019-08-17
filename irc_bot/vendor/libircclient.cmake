add_library(
    libircclient STATIC
    ${CMAKE_SOURCE_DIR}/vendor/libircclient/src/libircclient.c
    ${CMAKE_SOURCE_DIR}/vendor/libircclient/include/libircclient.h
    )

if (WIN32)
    target_compile_definitions(libircclient PRIVATE _CRT_NO_POSIX_ERROR_CODES)
endif()

target_include_directories(libircclient PRIVATE ${CMAKE_SOURCE_DIR}/vendor/libircclient/include/)

set_target_properties(libircclient PROPERTIES FOLDER "vendor")
