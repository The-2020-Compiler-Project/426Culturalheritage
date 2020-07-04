#ifndef _SRC_UTIL_ERROR_DIGNOSTIC_H
#define _SRC_UTIL_ERROR_DIGNOSTIC_H
#include <iostream>
#include <string>

void all_error(char * msg){
    std::cout<<"Error: "<<msg<<'\n';
    std::exit(1);
}

void all_error(const std::string &msg){
    std::cout<<"Error: "<<msg<<'\n';
    std::exit(1);
}
#endif
