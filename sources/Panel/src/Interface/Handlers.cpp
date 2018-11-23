#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Command.h"
#include "Message.h"
#include "Handlers.h"
#include "Display/Console.h"
#include "FrequencyMeter/FrequencyMeter.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Handlers::Processing(Message *msg)
{
    uint8 command = msg->TakeByte();

    static const struct StructFunc
    {
        typedef bool(*pFuncBpM)(Message *);
        pFuncBpM func;
        StructFunc(pFuncBpM f) : func(f) {}
    }
    functions[Command::Number] =
    {
        Handlers::Request,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::FreqMeasure,
        Handlers::Log,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E,
        Handlers::E
    };

    if (command < Command::Number)
    {
        /// Сюда сообщение передаётся уже без первого байта
        return functions[command].func(msg);
    }
    else
    {
        LOG_ERROR("Неправильная команда");
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Handlers::E(Message *)
{
    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Handlers::Request(Message *)
{
    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Handlers::FreqMeasure(Message *msg)
{
    FrequencyMeter::SetMeasure(msg->TakeWord());
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Handlers::Log(Message *msg)
{
    Console::AddString((pString)(msg->Data() + 1));
    return true;
}
