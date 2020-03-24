#include "defines.h"
#include "structs.h"
#include "common/Command.h"
#include "FDrive/FDrive_p.h"
#include "FDrive/Items.h"
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
    Items::Init();
}


bool FDrive::Handler::Processing(SimpleMessage *message)
{
    static const pFuncBV funcs[Command::Count] =
    {
        /* RequestData                  */ EmptyFuncBV,
        /* EnableChannel                */ EmptyFuncBV,
        /* SetFormWave                  */ EmptyFuncBV,
        /* SetFrequency                 */ EmptyFuncBV,
        /* SetAmplitude                 */ EmptyFuncBV,
        /* SetOffset                    */ EmptyFuncBV,
        /* SetDuration                  */ EmptyFuncBV,
        /* SetDutyRatio                 */ EmptyFuncBV,
        /* SetPhase                     */ EmptyFuncBV,
        /* RunReset                     */ EmptyFuncBV,
        /* ModeDebug                    */ EmptyFuncBV,
        /* SetDelay                     */ EmptyFuncBV,
        /* WriteRegister                */ EmptyFuncBV,
        /* SetDurationRise              */ EmptyFuncBV,
        /* SetDurationFall              */ EmptyFuncBV,
        /* SetDurationStady             */ EmptyFuncBV,
        /* SetDutyFactor                */ EmptyFuncBV,
        /* SetManipulation              */ EmptyFuncBV,
        /* SetManipulationDuration      */ EmptyFuncBV,
        /* SetManipulationPeriod        */ EmptyFuncBV,
        /* SetPacketPeriod              */ EmptyFuncBV,
        /* SetPacketNumber              */ EmptyFuncBV,
        /* SetStartMode                 */ EmptyFuncBV,
        /* SetPeriod                    */ EmptyFuncBV,
        /* SetPolarity                  */ EmptyFuncBV,
        /* SetManipulationMode          */ EmptyFuncBV,
        /* LoadFromDDS                  */ EmptyFuncBV,
        /* FreqMeasure                  */ EmptyFuncBV,
        /* Log                          */ EmptyFuncBV,
        /* FDrive_NumDirsAndFiles       */ Handler::GetNumDirsAndFiles,
        /* FDrive_Mount                 */ Handler::IsMount,
        /* FDrive_RequestDir            */ Handler::E,
        /* FDrive_RequestFile           */ Handler::RequestFile,
        /* Test                         */ EmptyFuncBV,
        /* SetKoeffCalibration          */ EmptyFuncBV,
        /* GetKoeffCalibration          */ EmptyFuncBV,
        /* FDrive_RequestFileSize       */ Handler::RequestFileSize,
        /* FDrive_RequestFileString     */ Handler::E,
        /* FDrive_LoadFromExtStorage    */ Handler::LoadFromExtStorage,
        /* FDrive_GetPictureDDS         */ Handler::E,
        /* SCPI_RecvData                */ EmptyFuncBV
    };

    msg = message;

    msg->ResetPointer();

    Command com(msg->TakeByte());

    return funcs[com]();
}


bool FDrive::Handler::IsMount()
{
    mounted = static_cast<Mount>(msg->TakeByte());
    Init();
    return true;
}


bool FDrive::Handler::GetNumDirsAndFiles()
{
    return Items::Handler::Processing(msg);
}


bool FDrive::Handler::E()
{
    return false;
}


bool FDrive::Handler::RequestFile()
{
    return Items::Handler::Processing(msg);
}


bool FDrive::Handler::RequestFileSize()
{
    return Items::Handler::Processing(msg);
}


bool FDrive::Handler::LoadFromExtStorage()
{
    inStateWaitCompleteLoad = false;

    return true;
}


void FDrive::PressUp()
{
    Items::PressUp();
}


void FDrive::PressDown()
{
    Items::PressDown();
}


void FDrive::PressChoose()
{
    inStateWaitCompleteLoad = true;

    Message::FDrive::LoadFromExtStorage(static_cast<uint8>(CURRENT_CHANNEL), static_cast<uint8>(Items::NumberCurrentFile()), FDrive::CurrentDirectory()).Transmit();

    File::SetDataToWave();
}


char *FDrive::CurrentDirectory()
{
    return directory;
}
