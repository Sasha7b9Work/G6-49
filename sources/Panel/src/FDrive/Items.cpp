#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "FDrivePanel.h"
#include "Items.h"
#include "Interface/InterfacePanel.h"
#include <cstring>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int  Items::numDirs = -1;
int  Items::numFiles = -1;
bool Items::requestIsSend = false;


#define NUM_ITEMS 25

struct StructFile
{
    char name[50];
    StructFile()
    {
        name[0] = 0;
    }
}
names[NUM_ITEMS];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Items::Init()
{
    numDirs = numFiles = -1;
    requestIsSend = false;
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        names[i].name[0] = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Items::SendRequest()
{
    Message message(Command::FDrive_NumDirsAndFiles, FDrive::directory);

    Interface::Send(&message);

    requestIsSend = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Items::Handler::Processing(Message *msg)
{
    msg->ResetPointer();

    uint8 command = msg->TakeByte();

    if (command == Command::FDrive_NumDirsAndFiles)
    {
        numDirs = (int)msg->TakeWord();
        numFiles = (int)msg->TakeWord();
        requestIsSend = false;
        return true;
    }
    else if (command == Command::FDrive_RequestFile)
    {
        int num = msg->TakeByte();
        std::strcpy(names[num].name, msg->String(2));
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendRequestForNameFile(int number)
{
    Message message(Command::FDrive_RequestFile, (uint8)number, FDrive::directory);

    Interface::Send(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Items::GetNameItem(int i)
{
    if (names[i].name[0])
    {
        return names[i].name;
    }
    else if (i < numFiles)
    {
        SendRequestForNameFile(i);
    }
    
    return 0;
}
