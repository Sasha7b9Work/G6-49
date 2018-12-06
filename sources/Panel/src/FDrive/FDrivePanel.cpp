#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "structs.h"
#include "Command.h"
#include "FDrivePanel.h"
#include "Items.h"
#include "Display/Painter.h"
#include "Display/Text.h" 
#include "Interface/InterfacePanel.h"
#include <cstdlib>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Примонтирована ли флешка
static bool isMounted = false;
/// Текущий файл
static int curItem = 0;

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

    if(!isMounted)
    {
        Text::DrawBigText(30, 110, 2, "Подключите флешку", Color::FILL);
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

    Painter::SetColor(Color::FILL);

    Text::SetUpperCase(false);

    for (int i = 0; i < 20; i++)
    {
        Items::GetNameItem(i).Draw(x + 5, y + 5 + i * 10);
    }

    Text::SetUpperCase(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::Handler::Processing(Message *msg)
{
    msg->ResetPointer();

    Command::E command = (Command::E)msg->TakeByte();

    if (command == Command::FDrive_Mount)
    {
        isMounted = (msg->TakeByte() != 0);
        Items::Init();
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

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::PressUp()
{
    LOG_WRITE("Вверх");

    if (curItem > 0)
    {
        curItem--;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::PressDown()
{
    LOG_WRITE("Вниз");

    if (curItem < Items::NumberFiles() - 1)
    {
        curItem++;
    }
}
