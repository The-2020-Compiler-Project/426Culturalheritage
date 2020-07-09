project(quadruple)

set(HEADER_FILES
        quadruple.h
        )

set(SOURCE_FILES
        quadruple.cpp
        )

add_library(quadrupleical STATIC ${SOURCE_FILES} ${HEADER_FILES})