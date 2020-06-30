project(big_int)

set(HEADER_FILES
        big_int.h
        )

set(SOURCE_FILES
       big_int.cpp
        )

add_library(big_int STATIC ${SOURCE_FILES} ${HEADER_FILES})