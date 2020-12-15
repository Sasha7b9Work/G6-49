#include "defines.h"
#include "Buffer.h"
#include <cstdlib>



Buffer::Buffer(uint _size) : data(nullptr), size(0)
{
    data = static_cast<uint8 *>(std::malloc(_size)); //-V2511
    size = _size;
}


Buffer::~Buffer()
{
    std::free(data); //-V2511
}

