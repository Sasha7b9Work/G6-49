#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "structs.h"
#include "Command.h"
#include "FDrivePanel.h"
#include "Interface/InterfacePanel.h"
#include <stdlib.h>
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Подготовить буфер для передачи
static uint8 *PrepareBufferForSend(uint size, uint8 command);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool FDrive::GetNumDirsAndFiles(pString directory, uint *numDirs, uint *numFiles)
{
    uint size = 1 + strlen(directory) + 1;

    Interface::Data data(size, Command::FDrive_NumDirsAndFiles);    // Подготавливаем и заполняем запрос

    strcpy((char *)data.GetData() + 1, directory);

    Interface::Data answer(0);                                      // Подготавливаем место для ответа

    LOG_WRITE("посылаю %d байт", size);
    bool result = Interface::Request(&data, &answer);               // Выполняем запрос, получем ответ

    *numDirs = BitSet32(answer.GetData() + 1).word;                 // Узнаём количество каталогов

    *numFiles = BitSet32(answer.GetData() + 5).word;                // Узнаём количество файлов

    return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::RequestNameDir(uint numDir, pString directory)
{
    uint size = 1 + 4 + strlen(directory) + 1;
    uint8 *data = PrepareBufferForSend(size, Command::FDrive_RequestDir);

    BitSet32 number(numDir);
    number.WriteToBuffer(data + 1);

    strcpy((char *)data + 5, directory);

    Interface::Send(data, size);

    free(data);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::RequestNameFile(uint numFile, pString directory)
{
    uint size = 1 + 4 + strlen(directory) + 1;
    uint8 *data = PrepareBufferForSend(size, Command::FDrive_RequestFile);

    BitSet32 number(numFile);
    number.WriteToBuffer(data + 1);

    strcpy((char *)data + 5, directory);

    Interface::Send(data, size);

    free(data);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 *PrepareBufferForSend(uint size, uint8 command)
{
    uint8 *buffer = (uint8 *)malloc(size);
    *buffer = command;
    return buffer;
}
