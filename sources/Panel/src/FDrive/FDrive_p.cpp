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


using namespace Primitives;


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
    ListFiles::Init();
}


void FDrive::Draw()
{
    int x = WaveGraphics::X();
    int y = WaveGraphics::Y(Chan::A) + 1;
    int width = WaveGraphics::Width() - 2;
    int height = WaveGraphics::Height() * 2;

    Rectangle(width, height).Fill(x, y, Color::BACK);

    if(mounted == Disconnect)
    {
        BigText("Подключите флешку", 2).Draw(30, 110, Color::FILL);
        return;
    }
    else if (mounted == Failed)
    {
        BigText("Флешка неисправна", 2).Draw(30, 110, Color::FILL);
        return;
    }

    if (ListFiles::NumberFiles() == -1)
    {
        ListFiles::SendRequest();
        return;
    }
    else if (ListFiles::WaitAnswer())
    {
        return;
    }

    ListFiles::Draw(x + 5, y + 5);

    if (inStateWaitCompleteLoad)
    {
        width = 200;
        height = 100;
        x = (Display::WIDTH - width) / 2;
        y = (Display::HEIGHT - height) / 2;

        Rectangle(width, height).DrawFilled(x, y, Color::BACK, Color::FILL);
    }
}


bool FDrive::Handler::Processing(SimpleMessage *message)
{
    msg = message;

    msg->ResetPointer();

    uint8 com = msg->TakeUINT8();

    pFuncBV func = E;

    switch(com)
    {
    case Command::FDrive_NumDirsAndFiles: func = Handler::GetNumDirsAndFiles; break;
    case Command::FDrive_Mount:           func = Handler::IsMount;            break;
    case Command::FDrive_RequestFile:     func = Handler::RequestFile;        break;
    case Command::FDrive_RequestFileSize: func = Handler::RequestFileSize;    break;
    case Command::FDrive_LoadDDSfromFile: func = Handler::LoadDDSfromFile;    break;
    }

    return func();
}


bool FDrive::Handler::IsMount()
{
    mounted = static_cast<Mount>(msg->TakeUINT8());
    Init();
    return true;
}


bool FDrive::Handler::GetNumDirsAndFiles()
{
    return ListFiles::Handler::Processing(msg);
}


bool FDrive::Handler::RequestFile()
{
    return ListFiles::Handler::Processing(msg);
}


bool FDrive::Handler::RequestFileSize()
{
    return ListFiles::Handler::Processing(msg);
}


bool FDrive::Handler::LoadDDSfromFile()
{
    inStateWaitCompleteLoad = false;

    return true;
}


void FDrive::PressUp()
{
    ListFiles::PressUp();
}


void FDrive::PressDown()
{
    ListFiles::PressDown();
}


void FDrive::PressChoose()
{
    inStateWaitCompleteLoad = true;

    Message::FDrive::LoadDDSfromFile(static_cast<uint8>(CURRENT_CHANNEL), static_cast<uint8>(ListFiles::NumberCurrentFile()), FDrive::CurrentDirectory()).Transmit();

    File::SetDataToWave();
}


char *FDrive::CurrentDirectory()
{
    return directory;
}


void FDrive::SaveScreenToFlash()
{

}
