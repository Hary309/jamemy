# find_library(IRCCLIENT_LIBRARY 
#             NAMES libircclient ircclient
#             HINTS "${CMAKE_SOURCE_DIR}/vendor/libircclient/bin"
#             )

FILE(GLOB LibIrcClient_src
    ${CMAKE_SOURCE_DIR}/vendor/libircclient/src/libircclient.c
    ${CMAKE_SOURCE_DIR}/vendor/libircclient/include/libircclient.h
)

add_library(
    libircclient STATIC
    ${LibIrcClient_src}
    )

if (WIN32)
    target_compile_definitions(libircclient PRIVATE _CRT_NO_POSIX_ERROR_CODES)
endif()

target_include_directories(libircclient PRIVATE ${CMAKE_SOURCE_DIR}/vendor/libircclient/include/)
