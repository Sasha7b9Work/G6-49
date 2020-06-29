#include "defines.h"
#include "log.h"
#include "common/Command.h"
#include "common/Messages.h"
#include "Handlers_p.h"
#include "Display/Console.h"
#include "FDrive/FDrive_p.h"
#include "FreqMeter/FreqMeter_p.h"
#include "SCPI/SCPI.h"


static bool E(SimpleMessage *)
{
    return false;
}


bool PHandlers::Processing(SimpleMessage *msg)
{
    typedef bool(*pFuncBpM)(SimpleMessage *);

    uint8 command = msg->TakeUINT8();

    pFuncBpM func = E;

    switch(command)
    {
    case Command::RequestData: func = PHandlers::Request;        break;
    case Command::FreqMeasure: func = PHandlers::FreqMeasure;    break;
    case Command::Log:         func = PHandlers::Log;            break;
    case Command::SCPI_Data:   func = SCPI::Handler::Processing; break;

    case Command::FDrive_NumDirsAndFiles:
    case Command::FDrive_Mount:
    case Command::FDrive_RequestDir:
    case Command::FDrive_RequestFile:
    case Command::FDrive_RequestFileSize:
    case Command::FDrive_RequestFileString:
    case Command::FDrive_LoadDDSfromFile:
    case Command::FDrive_RequestPictureDDS:
        func = FDrive::Handler::Processing;
        break;
    }

    if (command < Command::Count)
    {
        return func(msg);
    }
    else
    {
        LOG_ERROR("Неправильная команда");
    }

    return false;
}


bool PHandlers::Request(SimpleMessage *)
{
    return false;
}


bool PHandlers::FreqMeasure(SimpleMessage *msg)
{
    PFreqMeter::SetMeasure(msg->TakeUINT());
    return true;
}


bool PHandlers::Log(SimpleMessage *msg)
{
    Console::AddString(msg->String(1));
    return true;
}
