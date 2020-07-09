#include "tables.h"

// 标识符是否已存在
bool idenTable::exist( char *s) {
    auto iter = idTable.find(s);
    return iter != idTable.end();
}

// 返回标识符对应value
double idenTable::getValue( char *s ){
    return idTable[s];
}

// 向map中增加元素
void idenTable::append( char *s, double value ){
    idTable.insert( std::map<char* , double>::value_type(s,value) );
}

// 获取对应number下标
int constTable::getIndex(const double num) {
    for(int i=0; i<conTable.size(); i++){
        if( conTable[i] == num ){
            return i;
        }
    }
    return -1;
}

// 向常数表增加元素
void constTable::append( double num){
    conTable.push_back( num );
}