#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "RawMessage.h"
#include <cstdlib>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawMessage::RawMessage(uint size) : allocated(0), used(0)
{
    buffer = (uint8 *)std::malloc(size);
    if (buffer)
    {
        allocated = size;
        used = 0;
    }
}
