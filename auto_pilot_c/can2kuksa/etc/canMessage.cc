#include "canMessage.h"

CanMessage::CanMessage(int _id)
{
    id = _id;
}

void CanMessage::addKuksaItem(KuksaItem* tmp)
{
    items.push_back(tmp);
}


void CanMessage::updateValues(__u8 data[])
{
    for (int i = 0; i < items.size(); i++)
    {
        KuksaItem* tmp = items[i];
        int incomeValue = 0;
        int counter = 1;
        if (tmp->isIt2Byte)
        {
            counter = 2;
        }
        for (int i = 0; i < counter; i++)
        {
            for (int j = tmp->bitStart[i]; j >= tmp->bitEnd[i] ; j--)
            {
                incomeValue = incomeValue<<1;
                incomeValue += ((data[tmp->byte[i]]>>j)&1);
            }
        }
        
        incomeValue *= tmp->multiplicant;
        incomeValue += tmp->summation;
        // cout << tmp->path << ":  " << incomeValue << endl;
        if (tmp->value == incomeValue)
        {
            continue;
        }
        tmp->value = incomeValue;
        tmp->changedValue = true;
    }
    
}
