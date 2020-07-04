project(math)

add_subdirectory(big_int)
include_directories(big_int)
add_subdirectory(lexical)
include_directories(lexical)

add_executable(math main.cpp)
target_link_libraries(math big_int)
target_link_libraries(math lexical)