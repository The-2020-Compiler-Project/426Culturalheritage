//
// Created by emon100 on 2020/6/27 0027.
//
#include "big_int.h"

bool big_int::operator==(long long num) const {
    if ((num >= 0) != this->is_not_neg){
        return false;
    } else {//TODO
        return (*this)==big_int(num);
    }
}
bool big_int::operator==(big_int&&a) const {
    if(a.is_not_neg!=this->is_not_neg){
        return false;
    }
    return a.data==this->data;
}
bool big_int::operator==(const big_int &a) const {
    if(a.is_not_neg!=this->is_not_neg){
        return false;
    }
    return a.data==this->data;
}

big_int::big_int(const char *str,int len)
        :is_not_neg(str!=nullptr&&*str != '-')
{
    if(str == nullptr||*str=='\0'||len==0){//Empty str
        throw std::invalid_argument("empty string can't use");
    }
    if(!is_not_neg){//Skip '-'
        ++str;
        --len;
    }
    bool Allzero = false;
    while(*str=='0'){//Skip zero
        ++str;
        --len;
        Allzero = true;
    }
    if(*str=='\0'){//All zero
        if(Allzero){
            is_not_neg = true;
            data.push_back(0);
            return;
        }else{
            throw std::invalid_argument("Only '-'");
        }
    }

    data.reserve(10);
    int offset = len>0  ? len-1 : strlen(str)-1;

    while(offset>=0&&isdigit(*(str+offset))){
        int digit = *(str+offset) - '0';
        data.push_back(digit);
        --offset;
        if(!(offset>=0&&isdigit(*(str+offset)))) {
            break;
        }
        digit = *(str+offset) - '0';
        data.back()+=10*digit;
        --offset;
    }
    if(offset!=-1){
        throw std::invalid_argument("String can't contain character other than digits.");
    }
}

big_int::big_int(long long num)
        :is_not_neg(num >= 0) {
    num = llabs(num);
    data.reserve(10);
    if(num==0){
        data.push_back(0);
        return;
    }
    while(num != 0){
        data.push_back(num % 100);
        num/=100;
    }
}
/*
big_int& operator+=(){

}
 */

