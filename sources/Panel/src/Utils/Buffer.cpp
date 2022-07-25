// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Buffer.h"
#include <cstdlib>



Buffer::Buffer(uint _size) : data(nullptr), size(0)
{
    data = static_cast<uint8 *>(std::malloc(_size));
    size = _size;
}


Buffer::~Buffer()
{
    std::free(data);
}

