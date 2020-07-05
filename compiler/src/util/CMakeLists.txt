project(util)

set(HEADER_FILES
        )

set(SOURCE_FILES
        error_dignostic.cpp
        )

add_library(util STATIC ${SOURCE_FILES} ${HEADER_FILES})