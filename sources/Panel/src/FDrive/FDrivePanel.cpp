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

    for (int i = 0; i < 20; i++)
    {
        Text::DrawText(x + 5, y + 5 + i * 10, Items::GetNameItem(i));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::RequestNameDir(uint numDir)
{
//    uint size = 1 + 4 + std::strlen(directory) + 1;
    uint8 *data = 0; // PrepareBufferForSend(size, Command::FDrive_RequestDir);

    BitSet32 number(numDir);
    number.WriteToBuffer(data + 1);

    std::strcpy((char *)data + 5, directory);

//    Interface::Send(data, size);

    std::free(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::RequestNameFile(uint numFile)
{
    uint size = 1 + 4 + std::strlen(directory) + 1;
    uint8 *data = 0; // PrepareBufferForSend(size, Command::FDrive_RequestFile);

    BitSet32 number(numFile);
    number.WriteToBuffer(data + 1);

    std::strcpy((char *)data + 5, directory);

    Interface::Send(data, size);

    std::free(data);
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
        return true;
    }

    return false;
}
