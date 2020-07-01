#include "defines.h"
#include "log.h"
#include "common/Interface_p.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "FDrive_p.h"
#include "Items.h"
#include "File.h"
#include <cstring>


using namespace Primitives;


// Запрос имени файла с порядковым номером number
static void SendRequestForNameFile(int number);
// Запрос размера файла с порядковым номером number
//static void SendRequestForSizeFile(int number);
// Нарисовать i-й итем
static void DrawItem(int i, int x, int y, bool highlight);
// Возвращает имя i-го итема
static String GetNameItem(int i);
// Возвращает размер i-го итема
//static int GetSizeItem(int i);


static int numFiles;                    // Количество файлов в текущем каталоге
static int firstFile = 0;               // Этот файл первый в списке на экране
static int curFile = 0;                 // Текущий файл
bool ListFiles::requestIsSend = false;

#define NUM_FILES_ON_SCREEN 10          // Столько файлов помещается на экране

struct StructFile
{
    char name[50];
    uint8 nutUsed[2];
    int size;
    StructFile() : size(-1) { name[0] = 0; } //-V730
    void Clear() { name[0] = 0; size = -1; }
    void CopyFrom(const StructFile &file)
    {
        std::strcpy(name, file.name);
        size = file.size;
    }
}
files[NUM_FILES_ON_SCREEN];

// Теукущий файл
static File file;




void ListFiles::Init()
{
    firstFile = 0;
    numFiles = -1;
    requestIsSend = false;

    for (int i = 0; i < NUM_FILES_ON_SCREEN; i++)
    {
        files[i].Clear();
    }
}


int ListFiles::NumberCurrentFile()
{
    return curFile;
}


void ListFiles::SendRequest()
{
    Message::FDrive::NumDirsAndFiles(FDrive::CurrentDirectory()).Transmit();

    requestIsSend = true;
}


bool ListFiles::Handler::Processing(SimpleMessage *msg)
{
    msg->ResetPointer();

    uint8 command = msg->TakeUINT8();

    if (command == Command::FDrive_NumDirsAndFiles)
    {
        msg->TakeINT();             // Количество каталогов нам не нужно
        numFiles = msg->TakeINT(); //-V656
        requestIsSend = false;
        return true;
    }
    else if (command == Command::FDrive_RequestFile)
    {
        int num = msg->TakeUINT8();
        std::strcpy(files[num - firstFile].name, msg->String(2));

        if (num == curFile)
        {
            file.RequestFromPicture(num);
        }

        return true;
    }
    else if (command == Command::FDrive_RequestFileSize)
    {
        int num = msg->TakeUINT8();
        files[num - firstFile].size = msg->TakeINT();
        return true;
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
    
    Task *task = new Task(&message, ListFiles::Handler::Processing, EqualsRequestNameFile);
    
    PInterface::AddTask(task);
}


String GetNameItem(int i)
{
    if (files[i - firstFile].name[0] == 0)
    {
        SendRequestForNameFile(i);
    }
    
    return String(files[i - firstFile].name);
}


int ListFiles::NumberFiles()
{
    return numFiles;
}


void ListFiles::PressUp()
{
    if (curFile == 0)
    {
        return;
    }

    curFile--;

    if (curFile < firstFile)
    {
        firstFile--;

        for (int i = NUM_FILES_ON_SCREEN - 1; i > 0; i--)
        {
            files[i].CopyFrom(files[i - 1]);
        }

        files[0].Clear();
    }

    file.RequestFromPicture(curFile);
}



void ListFiles::PressDown()
{
    if (curFile == numFiles - 1)
    {
        return;
    }

    curFile++;

    if (curFile - firstFile > NUM_FILES_ON_SCREEN - 1)
    {
        firstFile++;

        for (int i = 1; i < NUM_FILES_ON_SCREEN; i++)
        {
            files[i - 1].CopyFrom(files[i]);
        }

        files[NUM_FILES_ON_SCREEN - 1].Clear();
    }

    file.RequestFromPicture(curFile);
}


void ListFiles::Draw(int x, int y)
{
    Font::ForceUpperCase(false);

    int lastFile = firstFile + NUM_FILES_ON_SCREEN;

    if (lastFile > numFiles)
    {
        lastFile = numFiles;
    }

    for (int i = firstFile; i < lastFile; i++)
    {
        DrawItem(i, x, y, (curFile == i));
        y += 10;
    }

    file.Draw(5, 235);

    Font::ForceUpperCase(true);

    String("%d/%d", curFile + 1, numFiles).Draw(75, 5, Color::FILL);
}


static void DrawItem(int i, int x, int y, bool highlight)
{
    Color color = Color::FILL;
    if (highlight)
    {
        Rectangle(230, 9).Fill(x - 1, y, color);
        color = Color::BACK;
    }
    GetNameItem(i).Draw(x, y, color);
    //String("%d", GetSizeItem(i)).Draw(x + 180, y);
}
