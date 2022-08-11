// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Buffer.h"
#include <cstdlib>


Buffer::Buffer(uint _size) : buffer(0), size(0)
{
    Allocate(_size);
}


Buffer::~Buffer()
{
    Free();
}


uint8 *Buffer::Data()
{
    return (uint8 *)buffer;
}


float *Buffer::DataFloat()
{
    return reinterpret_cast<float *>(Data());
}


uint Buffer::Size() const
{
    return size;
}


bool Buffer::ReSize(uint _size)
{
    Free();
    Allocate(_size);
    return (buffer != nullptr);
}


void Buffer::Free()
{
    std::free(buffer);

    size = 0;
}


void Buffer::Allocate(uint _size)
{
    buffer = std::malloc(_size);
    size = buffer ? _size : 0;
}

