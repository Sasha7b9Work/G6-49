#include "defines.h"
#include "log.h"
#include "Command.h"
#include "Message.h"
#include "Handlers_p.h"
#include "Display/Console.h"
#include "FDrive/FDrive_p.h"
#include "FreqMeter/FreqMeter_p.h"
#include "SCPI/SCPI.h"


namespace PHandlers
{
    bool E(SimpleMessage *);
    bool Request(SimpleMessage *);
    bool FreqMeasure(SimpleMessage *);
    bool Log(SimpleMessage *);
}



bool PHandlers::Processing(SimpleMessage *msg)
{
    typedef bool(*pFuncBpM)(SimpleMessage *);

    static const pFuncBpM funcs[Command::Count] =
    {
        /* RequestData               */ PHandlers::Request,
        /* EnableChannel             */ PHandlers::E,
        /* SetFormWave               */ PHandlers::E,
        /* SetFrequency              */ PHandlers::E,
        /* SetAmplitude              */ PHandlers::E,
        /* SetOffset                 */ PHandlers::E,
        /* SetDuration               */ PHandlers::E,
        /* SetDutyRatio              */ PHandlers::E,
        /* SetPhase                  */ PHandlers::E,
        /* RunReset                  */ PHandlers::E,
        /* ModeDebug                 */ PHandlers::E,
        /* SetDelay                  */ PHandlers::E,
        /* WriteRegister             */ PHandlers::E,
        /* SetDurationRise           */ PHandlers::E,
        /* SetDurationFall           */ PHandlers::E,
        /* SetDurationStady          */ PHandlers::E,
        /* SetDutyFactor             */ PHandlers::E,
        /* SetManipulation           */ PHandlers::E,
        /* SetManipulationDuration   */ PHandlers::E,
        /* SetManipulationPeriod     */ PHandlers::E,
        /* SetPacketPeriod           */ PHandlers::E,
        /* SetPacketNumber           */ PHandlers::E,
        /* SetStartMode              */ PHandlers::E,
        /* SetPeriod                 */ PHandlers::E,
        /* SetPolarity               */ PHandlers::E,
        /* SetManipulationMode       */ PHandlers::E,
        /* LoadFromDDS               */ PHandlers::E,
        /* FreqMeasure               */ PHandlers::FreqMeasure,
        /* Log                       */ PHandlers::Log,
        /* FDrive_NumDirsAndFiles    */ FDrive::Handler::Processing,
        /* FDrive_Mount              */ FDrive::Handler::Processing,
        /* FDrive_RequestDir         */ FDrive::Handler::Processing,
        /* FDrive_RequestFile        */ FDrive::Handler::Processing,
        /* Test                      */ PHandlers::E,
        /* SetKoeffCalibration       */ PHandlers::E,
        /* GetKoeffCalibration       */ PHandlers::E,
        /* FDrive_RequestFileSize    */ FDrive::Handler::Processing,
        /* FDrive_RequestFileString  */ FDrive::Handler::Processing,
        /* FDrive_LoadFromExtStorage */ FDrive::Handler::Processing,
        /* FDrive_GetPictureDDS      */ FDrive::Handler::Processing,
        /* SCPI_Data                 */ SCPI::Handler::Processing
    };

    uint8 command = msg->TakeByte();

    if (command < Command::Count)
    {
        /// Сюда сообщение передаётся уже без первого байта
        return funcs[command](msg);
    }
    else
    {
        LOG_ERROR("Неправильная команда");
    }

    return false;
}


bool PHandlers::E(SimpleMessage *)
{
    return false;
}


bool PHandlers::Request(SimpleMessage *)
{
    return false;
}


bool PHandlers::FreqMeasure(SimpleMessage *msg)
{
    PFreqMeter::SetMeasure(msg->TakeWord());
    return true;
}


bool PHandlers::Log(SimpleMessage *msg)
{
    Console::AddString(msg->String(1));
    return true;
}
