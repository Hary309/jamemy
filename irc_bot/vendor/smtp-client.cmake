
set(SMTP_CLIENT_SOURCE
    ${CMAKE_SOURCE_DIR}/vendor/smtp-client/src/smtp.c
    ${CMAKE_SOURCE_DIR}/vendor/smtp-client/src/smtp.h
)

add_library(smtp-client STATIC ${SMTP_CLIENT_SOURCE})
include_directories(smtp-client ${CMAKE_SOURCE_DIR}/vendor/smtp-client/src)

set_target_properties(smtp-client PROPERTIES FOLDER "vendor")
