#include "KuksaItem.h"

 KuksaItem::KuksaItem(string _path, bool _isIt2Byte,  int _byte[], int _bitStart[], int _bitEnd[], int _multiplicant, int _summation)
 {
    path = _path;
    value = -1;
    isIt2Byte = _isIt2Byte;
    byte[0] = _byte[0];
    byte[1] = _byte[1];
    bitStart[0] = _bitStart[0];
    bitStart[1] = _bitStart[1];
    bitEnd[0] = _bitEnd[0];
    bitEnd[1] = _bitEnd[1];
    multiplicant = _multiplicant;
    summation = _summation;
    changedValue = false;

 }