#include <gtest/gtest.h>
#include <iostream>
#include "lex.h"

TEST(lexical,single_token){
    Init_lexer("D:\\source\\426Culturalheritage\\compiler\\tests\\single_token.txt");
    auto i = get_token();
    EXPECT_STREQ(i.Name,";");
    EXPECT_EQ(i.token_type,TK_SEMICOLON);
    i = get_token();
    EXPECT_STREQ(i.Name,"EOF");
    EXPECT_EQ(i.token_type,TK_EOF);
    i = get_token();
    EXPECT_STREQ(i.Name,"EOF");
    EXPECT_EQ(i.token_type,TK_EOF);
    i = get_token();
    EXPECT_STREQ(i.Name,"EOF");
    EXPECT_EQ(i.token_type,TK_EOF);
}
