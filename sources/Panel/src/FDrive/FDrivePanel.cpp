#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "structs.h"
#include "Command.h"
#include "FDrivePanel.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Interface/InterfacePanel.h"
#include <cstdlib>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Примонтирована ли флешка
static bool isMounted = false;

class Items
{
public:
    /// true означает, что идёт ожидание ответа от устройства
    static bool WaitAnswer()
    {
        return requestIsSend;
    }
    /// Послать запрос на количество итемов
    static void SendRequest()
    {
        Message message(1, Command::FDrive_NumDirsAndFiles);
        Interface::Send(&message);
        requestIsSend = true;
    }
    /// Возвращает количество итемов. -1 - запрос не посылался
    static int NumberDirs()
    {
        return numDirs;
    }
    static int NumberFiles()
    {
        return numFiles;
    }

    class Handler
    {
    friend class FDrive;
    static bool Processing(Message *msg)
    {
        msg->ResetPointer();

        uint8 command = msg->TakeByte();

        if (command == Command::FDrive_NumDirsAndFiles)
        {
            numDirs = (int)msg->TakeWord();
            numFiles = (int)msg->TakeWord();
            return true;
        }

        return false;
    }
    };

private:
    static int numDirs;
    static int numFiles;
    static bool requestIsSend;

};

int  Items::numDirs = -1;
int  Items::numFiles = -1;
bool Items::requestIsSend = false;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    }
    else if (Items::WaitAnswer())
    {
        return;
    }

    Text::DrawFormatText(x + 10, y + 10, "Файлов - %d", Items::NumberFiles());

    Text::DrawFormatText(x + 10, y + 20, "Каталогов - %d", Items::NumberDirs());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::RequestNameDir(uint numDir, pString directory)
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
void FDrive::RequestNameFile(uint numFile, pString directory)
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
