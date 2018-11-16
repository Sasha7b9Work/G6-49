#pragma once


class Array
{
public:
    Array(uint size);
    ~Array();
    
    uint8 *Data();

    uint Size();

    uint CRC32();
private:
    
    uint8 *data;
    uint  size;
};
