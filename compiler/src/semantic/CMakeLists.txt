project(semantic)

set(HEADER_FILES
        semantic.h
        )

set(SOURCE_FILES
        semantic.cpp
        quadruple.cpp
        tables.cpp
        )

add_library(semantic STATIC ${SOURCE_FILES} ${HEADER_FILES})