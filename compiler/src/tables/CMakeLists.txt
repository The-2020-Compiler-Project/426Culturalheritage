project(tables)

set(HEADER_FILES
        tables.h
        )

set(SOURCE_FILES
        tables.cpp
        )

add_library(tablesical STATIC ${SOURCE_FILES} ${HEADER_FILES})