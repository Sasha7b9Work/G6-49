#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "structs.h"
#include "Command.h"
#include "FDrivePanel.h"
#include "Items.h"
#include "File.h"
#include "Display/Painter.h"
#include "Display/Text.h" 
#include "Interface/InterfacePanel.h"
#include <cstdlib>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum Mount
{
    Disconnect,
    Mounted,
    Failed
};

static Mount mounted  = Disconnect;

char FDrive::directory[255] = "\\";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FDrive::Init()
{
    std::strcpy(directory, "\\");
    Items::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

    if (Items::NumberDirs() == -1)
    {
        Items::SendRequest();
        return;
    }
    else if (Items::WaitAnswer())
    {
        return;
    }

    Items::Draw(x + 5, y + 5);

    //Items::CurrentFile()->Draw(5, 120);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::Handler::Processing(Message *msg)
{
    msg->ResetPointer();

    Command::E command = (Command::E)msg->TakeByte();

    if (command == Command::FDrive_Mount)
    {
        mounted = (Mount)msg->TakeByte();
        Init();
        return true;
    }
    else if (command == Command::FDrive_NumDirsAndFiles)
    {
        return Items::Handler::Processing(msg);
    }
    else if (command == Command::FDrive_RequestDir)
    {
        return true;
    }
    else if (command == Command::FDrive_RequestFile)
    {
        return Items::Handler::Processing(msg);
    }
    else if (command == Command::FDrive_RequestFileSize)
    {
        return Items::Handler::Processing(msg);
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::PressUp()
{
    Items::PressUp();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::PressDown()
{
    Items::PressDown();
}
