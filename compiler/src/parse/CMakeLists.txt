project(parse)

set(HEADER_FILES
        parse.h
        )

set(SOURCE_FILES
        parse.cpp
        )

add_library(parse STATIC ${SOURCE_FILES} ${HEADER_FILES})