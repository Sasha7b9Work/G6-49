#include "defines.h"
#include "log.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "FDrive_p.h"
#include "Items.h"
#include "File.h"
#include "Interface/Interface_p.h"
#include <cstring>


/// Запрос имени файла с порядковым номером number
static void SendRequestForNameFile(int number);
/// Запрос размера файла с порядковым номером number
//static void SendRequestForSizeFile(int number);
/// Нарисовать i-й итем
static void DrawItem(int i, int x, int y, bool highlight);
/// Возвращает имя i-го итема
static String GetNameItem(int i);
/// Возвращает размер i-го итема
//static int GetSizeItem(int i);


/// Количество файлов в текущем каталоге
static int numFiles;
bool Items::requestIsSend = false;

#define NUM_ITEMS 15

struct StructFile
{
    char name[50];
    uint8 nutUsed[2];
    int size;
    StructFile() : size(-1) { name[0] = 0; } //-V730
    void Clear() { name[0] = 0; size = -1; }
}
files[NUM_ITEMS];

/// Текущий файл
static int curItem = 0;
/// Количество каталогов в текущем каталоге
static int numDirs = -1;
/// Теукущий файл
static File file;




void Items::Init()
{
    numDirs = -1;
    numFiles = -1;
    requestIsSend = false;

    for (int i = 0; i < NUM_ITEMS; i++)
    {
        files[i].Clear();
    }
}


int Items::NumberCurrentFile()
{
    return curItem;
}


void Items::SendRequest()
{
    Message::FDrive::NumDirsAndFiles(FDrive::CurrentDirectory()).Transmit();

    requestIsSend = true;
}


bool Items::Handler::Processing(SimpleMessage *msg)
{
    msg->ResetPointer();

    uint8 command = msg->TakeUINT8();

    if (command == Command::FDrive_NumDirsAndFiles)
    {
        numDirs = msg->TakeINT();
        numFiles = msg->TakeINT(); //-V656
        requestIsSend = false;
        return true;
    }
    else if (command == Command::FDrive_RequestFile)
    {
        int num = msg->TakeUINT8();
        std::strcpy(files[num].name, msg->String(2));

        if (num == curItem)
        {
            file.Open(num);
        }

        return true;
    }
    else if (command == Command::FDrive_RequestFileSize)
    {
        int num = msg->TakeUINT8();
        files[num].size = msg->TakeINT();
        return true;
    }
    else
    {
        // здесь ничего
    }

    return false;
}


static bool EqualsRequestNameFile(Task *task1, Task *task2)
{
    SimpleMessage *msg1 = task1->GetMessage();

    SimpleMessage *msg2 = task2->GetMessage();

    uint8 com = Command::FDrive_RequestFile;

    return  (com == msg1->TakeUINT8()) &&
            (com == msg2->TakeUINT8()) &&
            (msg1->TakeUINT8() == msg2->TakeUINT8());
}


static void SendRequestForNameFile(int number)
{
    Message::FDrive::FileName message(static_cast<uint8>(number), FDrive::CurrentDirectory());
    
    Task *task = new Task(&message, Items::Handler::Processing, EqualsRequestNameFile);
    
    PInterface::AddTask(task);
}


String GetNameItem(int i)
{
    if (files[i].name[0] == 0)
    {
        SendRequestForNameFile(i);
    }
    
    return String(files[i].name);
}


int Items::NumberDirs()
{
    return numDirs;
}


void Items::PressUp()
{
    if (curItem > 0)
    {
        curItem--;
        file.Open(curItem);
    }
}



void Items::PressDown()
{
    if (curItem < numFiles - 1)
    {
        curItem++;
        file.Open(curItem);
    }
}


void Items::Draw(int x, int y)
{
    Text::SetUpperCase(false);

    for (int i = 0; i < numFiles; i++)
    {
        DrawItem(i, x, y, (curItem == i));
        y += 10;
    }

    file.Draw(5, 235);

    Text::SetUpperCase(true);
}


static void DrawItem(int i, int x, int y, bool highlight)
{
    Color color = Color::FILL;
    if (highlight)
    {
        Painter::FillRegion(x - 1, y, 230, 9, color);
        color = Color::BACK;
    }
    GetNameItem(i).Draw(x, y, color);
    //String("%d", GetSizeItem(i)).Draw(x + 180, y);
}
