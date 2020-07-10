project(gen)

set(HEADER_FILES
        gen.h
        )

set(SOURCE_FILES
        gen.cpp
        )


add_library(gen STATIC ${SOURCE_FILES} ${HEADER_FILES})