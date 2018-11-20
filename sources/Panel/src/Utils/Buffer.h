#pragma once
#include "defines.h"


class Buffer
{
public:
    Buffer(uint8 data0);
    Buffer(uint8 data0, uint8 data1);
    Buffer(uint8 data0, uint8 data1, uint8 data2);
    ~Buffer();
    int Length() const;
    uint8 *Data() const;
private:
    uint8 *data;
    int size;
};


