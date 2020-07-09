project(semantic)

set(HEADER_FILES
        semantic.h
        )

set(SOURCE_FILES
        semantic.cpp
        )

add_library(semantic STATIC ${SOURCE_FILES} ${HEADER_FILES})