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
/// Подготовить буфер для передачи
static uint8 *PrepareBufferForSend(uint size, uint8 command);
/// Примонтирована ли флешка
static bool isMounted = false;

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

	/*
    return;

    if(state == State::NeedRepaint)
    {
        uint numDirs = 0;
        uint numFiles = 0;
        if(!FDrive::GetNumDirsAndFiles(directory, &numDirs, &numFiles))
        {
            isConnected = false;
        }

        Painter::SetColor(Color::FILL);
        Text::DrawFormatText(50, 50, "каталогов - %d, файлов - %d", numDirs, numFiles);
    }

    DrawNameCurrentDir(x + 3, y + 1);
	*/
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::GetNumDirsAndFiles(pString directory, uint *numDirs, uint *numFiles)
{
    uint size = 1 + std::strlen(directory) + 1;

    Interface::Data data(size, Command::FDrive_NumDirsAndFiles);    // Подготавливаем и заполняем запрос

    std::strcpy((char *)data.GetData() + 1, directory);

    Interface::Data answer(0);                                      // Подготавливаем место для ответа

//    bool result = Interface::Request(&data, &answer);                  // Выполняем запрос, получем ответ

    *numDirs = BitSet32(answer.GetData() + 1).word;                 // Узнаём количество каталогов

    *numFiles = BitSet32(answer.GetData() + 5).word;                // Узнаём количество файлов

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::RequestNameDir(uint numDir, pString directory)
{
    uint size = 1 + 4 + std::strlen(directory) + 1;
    uint8 *data = PrepareBufferForSend(size, Command::FDrive_RequestDir);

    BitSet32 number(numDir);
    number.WriteToBuffer(data + 1);

    std::strcpy((char *)data + 5, directory);

    Interface::Send(data, size);

    std::free(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::RequestNameFile(uint numFile, pString directory)
{
    uint size = 1 + 4 + std::strlen(directory) + 1;
    uint8 *data = PrepareBufferForSend(size, Command::FDrive_RequestFile);

    BitSet32 number(numFile);
    number.WriteToBuffer(data + 1);

    std::strcpy((char *)data + 5, directory);

    Interface::Send(data, size);

    std::free(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 *PrepareBufferForSend(uint size, uint8 command)
{
    uint8 *buffer = (uint8 *)std::malloc(size);
    *buffer = command;
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::Handler::Processing(Message *msg)
{
    msg->ResetPointer();

    Command::E command = (Command::E)msg->TakeByte();

    if (command == Command::FDrive_Mount)
    {
        isMounted = (msg->TakeByte() != 0);
    }
    else if (command == Command::FDrive_NumDirsAndFiles)
    {

    }
    else if (command == Command::FDrive_RequestDir)
    {

    }
    else if (command == Command::FDrive_RequestFile)
    {

    }

    return false;
}
