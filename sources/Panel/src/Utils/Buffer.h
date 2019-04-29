#pragma once


class Buffer
{
public:
    Buffer(uint size);
    ~Buffer();
    uint8 *Data() { return data; };
    uint Size()   { return size; };
private:
    uint8 *data;
    uint size;
};
