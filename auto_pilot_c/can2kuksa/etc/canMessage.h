#ifndef CANMESSAGE_H
#define CANMESSAGE_H
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <linux/types.h>
#include <KuksaItem.h>
#include <math.h>
using namespace std;
class CanMessage
{
private:
    int id;
public:
    vector<KuksaItem*> items;
    CanMessage(int _id);
    void addKuksaItem(KuksaItem* tmp);
    void updateValues(__u8 data[]);
};
#endif