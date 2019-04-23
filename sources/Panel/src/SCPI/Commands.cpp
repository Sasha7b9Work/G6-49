#include "defines.h"
#include "SCPI/Commands.h"
#include "Display/Console.h"
#include "Utils/StringUtils.h"
#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SCPI::BaseCommand::CreateAndRun(uint8 *data, uint size)
{
    if (size == 0 || data[0] != ':')
    {
        return false;
    }

    uint start = 1;

    while (data[start] == ':')              // Выбираем начальыне ':'
    {
        start++;
    }

    uint end = start;

    while (end != 0x0d && end < size)       // Ищем завершающий 0x0d
    {
        end++;
    }

    if (data[end] != 0x0d)
    {
        return false;
    }

    SU::ToUpper(&data[start], end - start);

    return SCPI::BaseCommand::Run(&data[start], end - start);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SCPI::BaseCommand::Run(uint8 *data, uint size)
{
    if (SCPI::Command::IDN::Run(data, size))
    {
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SCPI::Command::IDN::Run(uint8 *data, uint size)
{
    if (size != 5)
    {
        return false;
    }

    if (std::memcmp(data, "*IDN?", size) == 0)
    {
        Console::AddString("*IDN?");
        return true;
    }

    return false;
}
