#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Command.h"
#include "Message.h"
#include "HandlersPanel.h"
#include "Display/Console.h"
#include "FDrive/FDrivePanel.h"
#include "FrequencyMeter/FrequencyMeter.h"
#include "SCPI/SCPI.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Handlers::Processing(SimpleMessage *msg)
{
    typedef bool(*pFuncBpM)(SimpleMessage *);

    static const pFuncBpM funcs[Command::Number] =
    {
        /* RequestData             */ Handlers::Request,
        /* EnableChannel           */ Handlers::E,
        /* SetFormWave             */ Handlers::E,
        /* SetFrequency            */ Handlers::E,
        /* SetAmplitude            */ Handlers::E,
        /* SetOffset               */ Handlers::E,
        /* SetDuration             */ Handlers::E,
        /* SetDutyRatio            */ Handlers::E,
        /* SetPhase                */ Handlers::E,
        /* RunReset                */ Handlers::E,
        /* ModeDebug               */ Handlers::E,
        /* SetDelay                */ Handlers::E,
        /* WriteRegister           */ Handlers::E,
        /* SetDurationRise         */ Handlers::E,
        /* SetDurationFall         */ Handlers::E,
        /* SetDurationStady        */ Handlers::E,
        /* SetDutyFactor           */ Handlers::E,
        /* SetManipulation         */ Handlers::E,
        /* SetManipulationDuration */ Handlers::E,
        /* SetManipulationPeriod   */ Handlers::E,
        /* SetPacketPeriod         */ Handlers::E,
        /* SetPacketNumber         */ Handlers::E,
        /* SetStartMode            */ Handlers::E,
        /* SetPeriod               */ Handlers::E,
        /* SetPolarity             */ Handlers::E,
        /* SetManipulationMode     */ Handlers::E,
        /* LoadFromDDS             */ Handlers::E,
        /* FreqMeasure             */ Handlers::FreqMeasure,
        /* Log                     */ Handlers::Log,
        /* FDrive_NumDirsAndFiles  */ FDrive::Handler::Processing,
        /* FDrive_Mount            */ FDrive::Handler::Processing,
        /* FDrive_RequestDir       */ FDrive::Handler::Processing,
        /* FDrive_RequestFile      */ FDrive::Handler::Processing,
        /* Test                    */ Handlers::E,
        /* SetKoeffCalibration     */ Handlers::E,
        /* GetKoeffCalibration     */ Handlers::E,
        /* FDrive_RequestFileSize  */ FDrive::Handler::Processing,
        /* FDrive_RequestFileString*/ FDrive::Handler::Processing,
        /* FDrive_LoadToFPGA       */ FDrive::Handler::Processing,
        /* FDrive_GetPictureDDS    */ FDrive::Handler::Processing,
        /* SCPI_Data               */ SCPI::Handler::Processing
    };

    uint8 command = msg->TakeByte();

    if (command < Command::Number)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Handlers::E(SimpleMessage *)
{
    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Handlers::Request(SimpleMessage *)
{
    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Handlers::FreqMeasure(SimpleMessage *msg)
{
    FrequencyMeter::SetMeasure(msg->TakeWord());
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Handlers::Log(SimpleMessage *msg)
{
    Console::AddString(msg->String(1));
    return true;
}
