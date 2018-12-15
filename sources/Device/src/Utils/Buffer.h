#pragma once
#include "defines.h"


/*
     ласс единственно предназначен дл€ автоматического выделени€/освобождени€ пам€ти из кучи
*/

class Buffer
{
public:
    Buffer(uint size);
    ~Buffer();
    bool ReSize(uint size);
    uint8 *Data();
    float *DataFloat();
    uint   Size() const;
private:
    void Allocate(uint size);
    void Free();
    void *buffer;
    uint size;
};
