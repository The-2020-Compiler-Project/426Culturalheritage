#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "big_int.h"

namespace testing{

    TEST(big_int_test, construct_ok){
        big_int a(0);
        EXPECT_EQ(a.data[0],0);
        big_int b(301);
        EXPECT_EQ(b.data[0],1);
        EXPECT_EQ(b.data[1],3);
        big_int c(-25551);
        EXPECT_EQ(c.data[0],51);
        EXPECT_EQ(c.data[1],55);
        EXPECT_EQ(c.data[2],2);
        big_int d("-00000000000000012345678910111213");
        auto temp = std::vector<char>{13,12,11,10,89,67,45,23,1};
        EXPECT_EQ(d.data,temp);
    }

    TEST(big_int_test,equal){
        EXPECT_EQ(big_int(0),big_int(0));
        EXPECT_EQ(big_int(1),big_int(1));
        EXPECT_EQ(big_int("000"),big_int(0));
        EXPECT_EQ(big_int("-0000"),big_int("0"));
        EXPECT_EQ(big_int("-00001"),big_int("-1"));
        EXPECT_EQ(big_int("100"),big_int(100));
        EXPECT_EQ(big_int("1000000000"),1000000000);
        EXPECT_EQ(big_int(0),0);
        EXPECT_EQ(big_int("00000"),0);
        EXPECT_EQ(big_int("-000012345678910111213141516171819"),big_int("-12345678910111213141516171819"));
    }
    TEST(big_int_test,construct_error){
        EXPECT_THROW(big_int(nullptr),std::invalid_argument);
        EXPECT_THROW(big_int(""),std::invalid_argument);
        EXPECT_THROW(big_int("-"),std::invalid_argument);
        EXPECT_THROW(big_int("----"),std::invalid_argument);
        EXPECT_THROW(big_int("\0\0\0"),std::invalid_argument);
        EXPECT_THROW(big_int("-000e3"),std::invalid_argument);
        EXPECT_THROW(big_int("-e333"),std::invalid_argument);
        EXPECT_THROW(big_int("e3333"),std::invalid_argument);
    }

/*
TEST(big_int_test,add_equal){
    EXPECT_EQ(big_int("99")+big_int("1"),big_int("100"));
    EXPECT_EQ(big_int(9)+big_int)
}
 */

};

