project(lexical)

set(HEADER_FILES
        lex.h
        )

set(SOURCE_FILES
        lex.cpp
        )

add_library(lexical STATIC ${SOURCE_FILES} ${HEADER_FILES})