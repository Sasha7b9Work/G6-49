#include "defines.h"
#include "log.h"
#include "structs.h"
#include "Command.h"
#include "FDrivePanel.h"
#include "Items.h"
#include "File.h"
#include "Display/Painter.h"
#include "Display/Text.h" 
#include "Settings/Settings.h"
#include "Hardware/Timer.h"
#include <cstdlib>


namespace FDrive
{
    namespace Handler
    {
        SimpleMessage *msg = nullptr;
        bool E();
        bool IsMount();
        bool GetNumDirsAndFiles();
        bool RequestFile();
        bool RequestFileSize();
        bool LoadFromExtStorage();
    }
}



enum Mount
{
    Disconnect,
    Mounted,
    Failed
};



/// Текущее состояние флешки
static Mount mounted = Disconnect;
/// Путь к текущему каталогу
static char directory[255];
/// Если true - идёт загрузка сигнала с флешки в память
static bool inStateWaitCompleteLoad = false;


void FDrive::Init()
{
    std::strcpy(directory, "\\");
    Items::Init();
}


void FDrive::Draw()
{
    int x = Wave::Graphics::X();
    int y = Wave::Graphics::Y(Chan::A) + 1;
    int width = Wave::Graphics::Width() - 2;
    int height = Wave::Graphics::Height() * 2;

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
