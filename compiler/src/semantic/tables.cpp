#include "semantic.h"

using namespace std;

// 标识符是否已存在
bool idenTable::exist(const char *s) {
    auto iter = idTable.find(s);
    return iter != idTable.end();
}

// 返回标识符对应value
std::string idenTable::getValue(const char *s) {
    return idTable[s];
}

// 向map中增加元素
void idenTable::append(const char *s, const string &value) {
    idTable[s] = value;
}

// 获取对应number下标
int constTable::getIndex(const double num) {
    for (int i = 0; i < conTable.size(); i++) {
        if (conTable[i] == num) {
            return i;
        }
    }
    return -1;
}

// 向常数表增加元素
void constTable::append(double num) {
    conTable.push_back(num);
}