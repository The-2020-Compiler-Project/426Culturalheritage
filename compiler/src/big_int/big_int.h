//
// Created by emon100 on 2020/3/2 0002.
//

#ifndef BIGDECIMAL_BIG_INT_H
#define BIGDECIMAL_BIG_INT_H

#include <vector>
#include <cstring>
#include <string>
#include <stdexcept>
#include <cmath>
/*
 * 数据结构：用vector<char>,每个char存放两位数。
 *
 */
class big_int {
private:
    bool is_not_neg;//true if number>=0
public:
    std::vector<char> data;//TODO
    big_int operator+(const big_int &a) const;
    big_int &operator+=(const big_int &a);
    friend big_int &operator+=(const long long num, const big_int &a);


    bool operator==(big_int &&a)const;
    bool operator==(const big_int &a)const;
    bool operator==(long long num)const;
    friend bool operator==(long long num,big_int &&t){return std::forward<big_int>(t) == num;}


    big_int() = delete;
    explicit big_int(short num):big_int(static_cast<long long>(num)){}
    explicit big_int(int num):big_int(static_cast<long long>(num)){}
    explicit big_int(long num):big_int(static_cast<long long> (num)){}
    explicit big_int(long long num);
    explicit big_int(std::string &&str):big_int(str.c_str(),str.length()){}
    explicit big_int(const char *str,int len=-1);

    big_int(big_int &&a) = default;
    big_int &operator=(big_int &&a) = default;
    big_int(const big_int &a) = default;
    big_int &operator=(const big_int &a) = default;
};


#endif //BIGDECIMAL_BIG_INT_H
