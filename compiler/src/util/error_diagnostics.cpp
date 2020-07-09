#ifndef _SRC_UTIL_ERROR_DIGNOSTIC_H
#define _SRC_UTIL_ERROR_DIGNOSTIC_H
#include <iostream>
#include <string>
#include "error_diagnostics.h"

void all_error(const char *msg) {
    std::cerr << "Error: " << msg << '\n';
    std::exit(1);
}

void all_error(const std::string &msg) {
    std::cerr << "Error: " << msg << '\n';
    std::exit(1);
}

void token_error(Token *token, const char *msg) {
    std::cerr << "Failed Token: " << token->Name << " Line num: " << token->line_num << " colomn_num: "
              << token->colomn_num << " Error: " << msg << '\n';
    std::exit(1);
}

void token_error(Token *token, const std::string &msg) {
    std::cerr << "Failed Token: " << token->Name << " Line num: " << token->line_num << " colomn_num: "
              << token->colomn_num << "Error: " << msg << '\n';
    std::exit(1);
}

#endif
