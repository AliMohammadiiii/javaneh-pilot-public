#ifndef KUKSAITEM_H
#define KUKSAITEM_H
#include <string>
using namespace std;

class KuksaItem
{
private:
public:
    string path;
    bool isIt2Byte;
    int byte[2];
    int bitStart[2];
    int bitEnd[2];
    int multiplicant;
    int summation;
    int size;
    bool changedValue;

    
    int value;
    KuksaItem(string _path, bool _isIt2Byte,  int _byte[], int _bitStart[], int _bitEnd[], int _multiplicant, int _summation);
};
#endif
