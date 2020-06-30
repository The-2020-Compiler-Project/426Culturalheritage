#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "big_int.h"

TEST(big_int_test,equal2){
    big_int a("123123");
    big_int b("-123123");
    EXPECT_EQ(a,b);
}
