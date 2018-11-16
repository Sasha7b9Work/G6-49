#include "defines.h"
#include "Array.h"
#include "Hardware/CPU.h"


Array::Array(uint _size) : size(_size)
{
    size = size / 4;
    size *= 4;
    data = (uint8 *)malloc(size);
}

Array::~Array()
{
    free(data);
}

uint8 *Array::Data()
{
    return data;
}

uint Array::Size()
{
    return size;
}

uint Array::CRC32()
{
    return CPU::CRC32::Calculate(data, size);
}
