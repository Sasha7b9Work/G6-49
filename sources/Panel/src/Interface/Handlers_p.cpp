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

    static const pFuncBpM funcs[Command::Count] =
    {
        /* RequestData                */ PHandlers::Request,
        /* EnableChannel              */ E,
        /* SetFormWave                */ E,
        /* SetFrequency               */ E,
        /* SetAmplitude               */ E,
        /* SetOffset                  */ E,
        /* SetDuration                */ E,
        /* SetDutyRatio               */ E,
        /* SetPhase                   */ E,
        /* ModeDebug                  */ E,
        /* SetDelay                   */ E,
        /* WriteRegister              */ E,
        /* SetDurationRise            */ E,
        /* SetDurationFall            */ E,
        /* SetDurationStady           */ E,
        /* SetDutyFactor              */ E,
        /* SetManipulation            */ E,
        /* SetManipulationDuration    */ E,
        /* SetManipulationPeriod      */ E,
        /* SetPacketPeriod            */ E,
        /* SetPacketNumber            */ E,
        /* SetStartMode               */ E,
        /* SetPeriod                  */ E,
        /* SetPolarity                */ E,
        /* LoadFromDDS                */ E,
        /* FreqMeasure                */ PHandlers::FreqMeasure,
        /* Log                        */ PHandlers::Log,
        /* FDrive_NumDirsAndFiles     */ FDrive::Handler::Processing,
        /* FDrive_Mount               */ FDrive::Handler::Processing,
        /* FDrive_RequestDir          */ FDrive::Handler::Processing,
        /* FDrive_RequestFile         */ FDrive::Handler::Processing,
        /* Test                       */ E,
        /* SetKoeffCalibration        */ E,
        /* GetKoeffCalibration        */ E,
        /* FDrive_RequestFileSize     */ FDrive::Handler::Processing,
        /* FDrive_RequestFileString   */ FDrive::Handler::Processing,
        /* FDrive_LoadFromExtStorage  */ FDrive::Handler::Processing,
        /* FDrive_GetPictureDDS       */ FDrive::Handler::Processing,
        /* SCPI_Data                  */ SCPI::Handler::Processing,
        /* PortCPU                    */ E,
        /* CalibrationLoad            */ E,
        /* CalibrationSet             */ E,

        /* StartApplication           */ E,
        /* RequestUpgrade             */ E,
        /* PortionUpgradeDevice       */ E,
        /* AnswerUpgradePanel         */ E,
        /* RequestPortionUpgradePanel */ E,
        /* AnswerPortionUpgradePanel  */ E
    };

    uint8 command = msg->TakeUINT8();

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
