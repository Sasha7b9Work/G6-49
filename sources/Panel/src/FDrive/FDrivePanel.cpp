#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Command.h"
#include "FDrivePanel.h"
#include "Generator/InterfacePanel.h"
#include <stdlib.h>
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool isConnected = false;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FDrive::Init()
{
    Graphics::Init();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::RequestNumDirsAndFiles(pString directory)
{
    uint size = strlen(directory) + 2;

    uint8 *data = (uint8 *)malloc(size);
    data[0] = Command::FDrive_NumDirsAndFiles;
    memcpy(data + 1, directory, strlen(directory));
    data[size - 1] = 0;

    Interface::Send(data, size);

    free(data);
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
