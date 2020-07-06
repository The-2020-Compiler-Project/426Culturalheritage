project(parse)

set(HEADER_FILES
        parse.h
        Grammar.h
        )

set(SOURCE_FILES
        parse.cpp
        Grammar.cpp
        )

add_library(parse STATIC ${SOURCE_FILES} ${HEADER_FILES})