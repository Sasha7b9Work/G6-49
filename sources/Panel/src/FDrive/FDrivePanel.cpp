#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "structs.h"
#include "Command.h"
#include "FDrivePanel.h"
#include "Generator/InterfacePanel.h"
#include <stdlib.h>
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool isConnected = false;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Подготовить буфер для передачи
static uint8 *PrepareBufferForSend(uint size, uint8 command);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FDrive::Init()
{
    Graphics::Init();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::RequestNumDirsAndFiles(pString directory)
{
    uint size = 1 + strlen(directory) + 1;
    uint8 *data = PrepareBufferForSend(size, Command::FDrive_NumDirsAndFiles);

    memcpy(data + 1, directory, strlen(directory));
    data[size - 1] = 0;

    Interface::Send(data, size);

    free(data);
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::SetConnected(bool connected)
{
    isConnected = connected;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::IsConnected()
{
    return isConnected;
}
