#ifndef COMPILER_TABLES_H
#define COMPILER_TABLES_H

#include <string>
#include <vector>
#include <map>
#include "cstring"
class idenTable{
public:
    std::map<char*,double> idTable;

    bool exist( char* s );
    double getValue( char *s );
    void append ( char *s, double value=0);

    idenTable(){};
    ~idenTable() = default;

};

class constTable{
public:
    std::vector<double > conTable;

    int getIndex( double num );
    void append( double num);

    constTable(){};
    ~constTable() = default;
};
#endif //COMPILER_TABLES_H
