#include "defines.h"
#include "log.h"
#include "structs.h"
#include "common/Command.h"
#include "FDrive_p.h"
#include "Items.h"
#include "File.h"
#include "Display/Painter.h"
#include "Display/Text.h" 
#include "Display/WaveGraphics.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"
#include <cstdlib>


FDrive::View   FDrive::view;
SimpleMessage *FDrive::Handler::msg = nullptr;


FDrive::Mount FDrive::mounted = Disconnect;
char          FDrive::directory[255];
bool          FDrive::inStateWaitCompleteLoad = false;

static bool E()
{
    return false;
}


void FDrive::Init()
{
    std::strcpy(directory, "\\");
    Items::Init();
}


void FDrive::Draw()
{
    int x = WaveGraphics::X();
    int y = WaveGraphics::Y(Chan::A) + 1;
    int width = WaveGraphics::Width() - 2;
    int height = WaveGraphics::Height() * 2;

    Painter::FillRegion(x, y, width, height, Color::BACK);

    if(mounted == Disconnect)
    {
        Text::DrawBigText(30, 110, 2, "Подключите флешку", Color::FILL);
        return;
    }
    else if (mounted == Failed)
    {
        Text::DrawBigText(30, 110, 2, "Флешка неисправна", Color::FILL);
        return;
    }
    else
    {
        // здесь ничего
    }

    if (Items::NumberDirs() == -1)
    {
        Items::SendRequest();
        return;
    }
    else if (Items::WaitAnswer())
    {
        return;
    }
    else
    {
        // здесь ничего
    }

    Items::Draw(x + 5, y + 5);

    if (inStateWaitCompleteLoad)
    {
        width = 200;
        height = 100;
        x = (SCREEN_WIDTH - width) / 2;
        y = (SCREEN_HEIGHT - height) / 2;

        Painter::DrawFilledRectangle(x, y, width, height, Color::BACK, Color::FILL);
    }
}


bool FDrive::Handler::Processing(SimpleMessage *message)
{
    static const pFuncBV funcs[Command::Count] =
    {
        /* RequestData                  */ E,
        /* EnableChannel                */ E,
        /* SetFormWave                  */ E,
        /* SetFrequency                 */ E,
        /* SetAmplitude                 */ E,
        /* SetOffset                    */ E,
        /* SetDuration                  */ E,
        /* SetDutyRatio                 */ E,
        /* SetPhase                     */ E,
        /* RunReset                     */ E,
        /* ModeDebug                    */ E,
        /* SetDelay                     */ E,
        /* WriteRegister                */ E,
        /* SetDurationRise              */ E,
        /* SetDurationFall              */ E,
        /* SetDurationStady             */ E,
        /* SetDutyFactor                */ E,
        /* SetManipulation              */ E,
        /* SetManipulationDuration      */ E,
        /* SetManipulationPeriod        */ E,
        /* SetPacketPeriod              */ E,
        /* SetPacketNumber              */ E,
        /* SetStartMode                 */ E,
        /* SetPeriod                    */ E,
        /* SetPolarity                  */ E,
        /* SetManipulationMode          */ E,
        /* LoadFromDDS                  */ E,
        /* FreqMeasure                  */ E,
        /* Log                          */ E,
        /* FDrive_NumDirsAndFiles       */ Handler::GetNumDirsAndFiles,
        /* FDrive_Mount                 */ Handler::IsMount,
        /* FDrive_RequestDir            */ E,
        /* FDrive_RequestFile           */ Handler::RequestFile,
        /* Test                         */ E,
        /* SetKoeffCalibration          */ E,
        /* GetKoeffCalibration          */ E,
        /* FDrive_RequestFileSize       */ Handler::RequestFileSize,
        /* FDrive_RequestFileString     */ E,
        /* FDrive_LoadFromExtStorage    */ Handler::LoadFromExtStorage,
        /* FDrive_GetPictureDDS         */ E,
        /* SCPI_RecvData                */ E,
        /* PortCPU                      */ E,
        /* CalibrationLoad              */ E,
        /* CalibrationSet               */ E,
        /* StartApplication             */ E,
        /* RequestUpdate                */ E
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
