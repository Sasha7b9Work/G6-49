// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "common/Interface_p.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "FDrive_p.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Items.h"
#include "File.h"
#include <cstring>


using namespace Primitives;


namespace ListFiles
{
    // ������ ������. ��������� �����
    static bool requestIsSend = false;

    static const int NUM_FILES_ON_SCREEN = 10;  // ������� ������ ���������� �� ������

    static int numFiles;                        // ���������� ������ � ������� ��������
    static int firstFile = 0;                   // ���� ���� ������ � ������ �� ������
    static int curFile = 0;                     // ������� ����
    static uint timeStopUnderFile = _TIME_MS;   // ����� ��������� ������� ��� ������. ����� ��������� ����� ����� ��������� ��������� ����� �������� ������ �� �����������

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

    // �������� ����
    static File file;

    // ���������� ��� i-�� �����
    static String GetNameItem(int i);

    // ���������� i-� ����
    static void DrawItem(int i, int x, int y, bool highlight);

    // ������ ����� ����� � ���������� ������� number
    static void SendRequestForNameFile(int number);
}


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


bool ListFiles::WaitAnswer()
{
    return requestIsSend;
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
        msg->TakeINT();             // ���������� ��������� ��� �� �����
        numFiles = msg->TakeINT(); //-V656
        requestIsSend = false;
        return true;
    }
    else if (command == Command::FDrive_RequestFile)
    {
        int _num = msg->TakeUINT8();
        std::strcpy(files[_num - firstFile].name, msg->String(2));

        if (_num == curFile)
        {
            file.RequestFromPicture(_num);
        }

        return true;
    }
    else if (command == Command::FDrive_RequestFileSize)
    {
        files[msg->TakeUINT8() - firstFile].size = msg->TakeINT();
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


static void ListFiles::SendRequestForNameFile(int number)
{
    Message::FDrive::FileName message((uint8)number, FDrive::CurrentDirectory());
    
    Task *task = new Task(&message, ListFiles::Handler::Processing, EqualsRequestNameFile);
    
    PInterface::AddTask(task);
}


String ListFiles::GetNameItem(int i)
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

    timeStopUnderFile = _TIME_MS;
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

    timeStopUnderFile = _TIME_MS;
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

    if (timeStopUnderFile != 0 && _TIME_MS - timeStopUnderFile >= 500)
    {
        file.RequestFromPicture(curFile);
        timeStopUnderFile = 0;
    }
}


static void ListFiles::DrawItem(int i, int x, int y, bool highlight)
{
    Color color = Color::FILL;
    if (highlight)
    {
        Rectangle(230, 9).Fill(x - 1, y, color);
        color = Color::BACK;
    }

    GetNameItem(i).Draw(x, y, color);
}
