#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "SCPI.h"
#include "Hardware/VCP.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#include <cstring>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SCPI::AddNewData(uint8 *buffer, uint length)
{
    SU::ToUpper(buffer, length);
    
    Buffer data(length + 1);
    std::memcpy(data.Data(), buffer, length);
    data.Data()[length] = 0;					                         // -V108
    
    if (SU::EqualsStrings(buffer, "*IDN?", 5))
    {
        char *answer = "MNIPI, G6-49, DDS V.1.3";
        VCP::SendStringAsynch(answer);
    }
}
