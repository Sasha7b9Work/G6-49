// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "common/Command.h"
#include "common/Messages.h"
#include "Interface/Handlers_p.h"
#include "Display/Console.h"
#include "FDrive/FDrive_p.h"
#include "FreqMeter/FreqMeter_p.h"
#include "SCPI/SCPI.h"


namespace PHandlers
{
    static bool Request(SimpleMessage *);
    static bool FreqMeasure(SimpleMessage *);
    static bool Log(SimpleMessage *);
    static bool RequestPictureDDSfromStorage(SimpleMessage *);
}


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
    case Command::RequestData:                  func = Request;                      break;
    case Command::FreqMeasure:                  func = FreqMeasure;                  break;
    case Command::Log:                          func = Log;                          break;
    case Command::SCPI_Data:                    func = SCPI::Handler::Processing;    break;
    case Command::RequestPictureDDSfromStorage: func = RequestPictureDDSfromStorage; break;

    case Command::FDrive_NumDirsAndFiles:
    case Command::FDrive_Mount:
    case Command::FDrive_RequestDir:
    case Command::FDrive_RequestFile:
    case Command::FDrive_RequestFileSize:
    case Command::FDrive_RequestFileString:
    case Command::FDrive_LoadDDSfromFile:
    case Command::FDrive_RequestPictureDDSfromFile:
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


static bool PHandlers::Request(SimpleMessage *)
{
    return false;
}


static bool PHandlers::FreqMeasure(SimpleMessage *msg)
{
    PFreqMeter::SetMeasure(msg->TakeUINT());
    return true;
}


static bool PHandlers::Log(SimpleMessage *msg)
{
    Console::AddString(msg->String(1));
    return true;
}


static bool PHandlers::RequestPictureDDSfromStorage(SimpleMessage *message)
{
    Chan ch(message->TakeUINT8());

    uint8 pic[240];

    message->TakeRemainigData(pic);

    Form::SetFormFlash(ch, pic);

    return true;
}
