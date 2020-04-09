#include "defines.h"
#include "common/Command.h"
#include "common/Messages.h"
#include "Updater_pl.h"
#include "FDrive/FDrive_p.h"
#include "Interface/Handlers_pl.h"
#include <stm32f429xx.h>


static bool OnStartMainApplication(SimpleMessage *message);


static bool E(SimpleMessage *)
{
    return false;
}


bool PHandlers::Processing(SimpleMessage *msg)
{
    typedef bool(*pFuncBpM)(SimpleMessage *);

    static const pFuncBpM funcs[Command::Count] =
    {
        /* RequestData               */ PHandlers::Request,
        /* EnableChannel             */ E,
        /* SetFormWave               */ E,
        /* SetFrequency              */ E,
        /* SetAmplitude              */ E,
        /* SetOffset                 */ E,
        /* SetDuration               */ E,
        /* SetDutyRatio              */ E,
        /* SetPhase                  */ E,
        /* ModeDebug                 */ E,
        /* SetDelay                  */ E,
        /* WriteRegister             */ E,
        /* SetDurationRise           */ E,
        /* SetDurationFall           */ E,
        /* SetDurationStady          */ E,
        /* SetDutyFactor             */ E,
        /* SetManipulation           */ E,
        /* SetManipulationDuration   */ E,
        /* SetManipulationPeriod     */ E,
        /* SetPacketPeriod           */ E,
        /* SetPacketNumber           */ E,
        /* SetStartMode              */ E,
        /* SetPeriod                 */ E,
        /* SetPolarity               */ E,
        /* SetManipulationMode       */ E,
        /* LoadFromDDS               */ E,
        /* FreqMeasure               */ E,
        /* Log                       */ E,
        /* FDrive_NumDirsAndFiles    */ FDrive::Handler::Processing,
        /* FDrive_Mount              */ FDrive::Handler::Processing,
        /* FDrive_RequestDir         */ FDrive::Handler::Processing,
        /* FDrive_RequestFile        */ FDrive::Handler::Processing,
        /* Test                      */ E,
        /* SetKoeffCalibration       */ E,
        /* GetKoeffCalibration       */ E,
        /* FDrive_RequestFileSize    */ FDrive::Handler::Processing,
        /* FDrive_RequestFileString  */ FDrive::Handler::Processing,
        /* FDrive_LoadFromExtStorage */ FDrive::Handler::Processing,
        /* FDrive_GetPictureDDS      */ FDrive::Handler::Processing,
        /* SCPI_Data                 */ E,
        /* PortCPU                   */ E,
        /* CalibrationLoad           */ E,
        /* CalibrationSet            */ E,
        /* StartApplication          */ OnStartMainApplication,
        /* RequestUpdate             */ E,
        /* PortionUpdateDevice       */ Updater::Handler,
        /* PortionUpgradePanel       */ E
    };
   
    uint8 command = msg->TakeByte();
    
    if (command < Command::Count)
    {
        /// —юда сообщение передаЄтс€ уже без первого байта
        return funcs[command](msg);
    }

    return false;
}


bool PHandlers::Request(SimpleMessage *)
{
    return false;
}


static bool OnStartMainApplication(SimpleMessage *)
{
#define MAIN_PROGRAM_START_ADDRESS  (uint)0x8020000

    typedef void(*pFunction)();

    __disable_irq();

    pFunction JumpToApplication;

    JumpToApplication = (pFunction)(*(__IO uint *)(MAIN_PROGRAM_START_ADDRESS + 4));

    __set_MSP(*(__IO uint *)MAIN_PROGRAM_START_ADDRESS);

    __enable_irq();

    JumpToApplication();

    return true;
}
