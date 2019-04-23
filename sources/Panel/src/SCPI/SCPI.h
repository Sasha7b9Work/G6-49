#pragma once
#include "Message.h"


struct StructCommand
{
    char *symbols;
    bool (*func)(uint8 *);
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace SCPI
{
    class Handler
    {
    public:
        static bool Processing(SimpleMessage *msg);
    };

    /// Выполнение команды. Если команда выполнена, выполняется адрес последнего символа команды (это символ завершения строки 0x0d)
    uint8 *ProcessingCommand(const StructCommand *commands, uint8 *data);

    bool ProcessIDN(uint8 *data);
};
