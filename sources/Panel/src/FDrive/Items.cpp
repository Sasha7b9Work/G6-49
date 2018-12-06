#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "FDrivePanel.h"
#include "Items.h"
#include "Interface/InterfacePanel.h"
#include <cstring>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Запрос имени файла с порядковым номером number
static void SendRequestForNameFile(int number);
/// Запрос размера файла с порядковым номером number
static void SendRequestForSizeFile(int number);
/// Нарисовать i-й итем
static void DrawItem(int i, int x, int y, bool highlight);
/// Возвращает имя i-го итема
static String GetNameItem(int i);
/// Возвращает размер i-го итема
static int GetSizeItem(int i);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Количество файлов в текущем каталоге
static int numFiles;
bool Items::requestIsSend = false;


#define NUM_ITEMS 25

struct StructFile
{
    char name[50];
    int size;
    StructFile() : size(-1)
    {
        name[0] = 0;
    }
}
names[NUM_ITEMS];

/// Текущий файл
static int curItem = 0;
/// Количество каталогов в текущем каталоге
static int numDirs = -1;


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
    else if (command == Command::FDrive_RequestFileSize)
    {
        int num = msg->TakeByte();
        names[num].size = (int)msg->TakeWord();
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
static void SendRequestForSizeFile(int number)
{
    Message message(Command::FDrive_RequestFileSize, (uint8)number, FDrive::directory);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String GetNameItem(int i)
{
    if (names[i].name[0])
    {
        return String(names[i].name);
    }
    else if (i < numFiles)
    {
        SendRequestForNameFile(i);
    }
    
    return String();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int GetSizeItem(int i)
{
    if (names[i].size == -1)
    {
        SendRequestForSizeFile(i);
    }

    return names[i].size;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Items::NumberDirs()
{
    return numDirs;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Items::PressUp()
{
    if (curItem > 0)
    {
        curItem--;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Items::PressDown()
{
    if (curItem < numFiles - 1)
    {
        curItem++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Items::DrawItems(int x, int y)
{
    Text::SetUpperCase(false);

    for (int i = 0; i < numFiles; i++)
    {
        DrawItem(i, x, y, (curItem == i));
        y += 10;
    }

    Text::SetUpperCase(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawItem(int i, int x, int y, bool highlight)
{
    Color color = Color::FILL;
    if (highlight)
    {
        Painter::FillRegion(x - 1, y, 200, 9, color);
        color = Color::BACK;
    }
    GetNameItem(i).Draw(x, y, color);
    String("%d", GetSizeItem(i)).Draw(x + 180, y);
}
