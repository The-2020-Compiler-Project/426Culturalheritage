//
// Created by emon100 on 2020/7/6 0006.
//

#ifndef COMPILER_DIAGNOSTICS_H
#define COMPILER_DIAGNOSTICS_H

#include <string>
#include "lex.h"

void all_error(const char *msg);

void all_error(const std::string &msg);

void token_error(Token *token, const char *msg);

void token_error(Token *token, const std::string &msg);

#endif //COMPILER_DIAGNOSTICS_H
