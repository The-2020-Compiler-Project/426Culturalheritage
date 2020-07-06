project(util)

set(HEADER_FILES
        error_diagnostics.h
        )

set(SOURCE_FILES
        error_diagnostics.cpp
        )

add_library(util STATIC ${SOURCE_FILES} ${HEADER_FILES})