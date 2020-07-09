project(tables)

set(HEADER_FILES
        tables.h
        )

set(SOURCE_FILES
        tables.cpp
        )

add_library(tables STATIC ${SOURCE_FILES} ${HEADER_FILES})