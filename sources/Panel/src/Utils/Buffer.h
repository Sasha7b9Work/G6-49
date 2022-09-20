// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Buffer
{
public:
    Buffer(uint size);
    ~Buffer();
    uint8 *Data()    const { return data; };
    char *DataChar() const { return (char *)data; }
    uint Size()      const { return size; };
private:
    uint8 *data;
    uint size;
};
