#include "defines.h"
#include "structs.h"
#include "common/Command.h"
#include "FDrive/FDrive_p.h"
#include "File.h"
#include "Hardware/Timer.h"
#include <cstdlib>
#include <cstring>


FDrive::View   FDrive::view;
SimpleMessage *FDrive::Handler::msg = nullptr;


FDrive::Mount FDrive::mounted = Disconnect;
char          FDrive::directory[255];
bool          FDrive::inStateWaitCompleteLoad = false;


void FDrive::Init()
{
    std::strcpy(directory, "\\");
}

static bool E()
{
    return false;
}


bool FDrive::Handler::Processing(SimpleMessage *message)
{
    static const pFuncBV funcs[Command::Count] =
    {
        /* RequestData                */ E,
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
        /* FreqMeasure                */ E,
        /* Log                        */ E,
        /* FDrive_NumDirsAndFiles     */ E,
        /* FDrive_Mount               */ Handler::IsMount,
        /* FDrive_RequestDir          */ E,
        /* FDrive_RequestFile         */ E,
        /* Test                       */ E,
        /* FDrive_RequestFileSize     */ E,
        /* FDrive_RequestFileString   */ E,
        /* FDrive_LoadFromExtStorage  */ E,
        /* FDrive_GetPictureDDS       */ E,
        /* SCPI_RecvData              */ E,
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

    msg = message;

    msg->ResetPointer();

    Command com(msg->TakeUINT8());

    return funcs[com]();
}


bool FDrive::Handler::IsMount()
{
    mounted = static_cast<Mount>(msg->TakeUINT8());
    Init();
    return true;
}


char *FDrive::CurrentDirectory()
{
    return directory;
}
