#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "structs.h"
#include "FDrivePanel.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Hardware/CPU.h"
#include "Utils/Debug.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RECS_ON_PAGE    10

/// Номер первого выведенного каталога. Всего может быть выведено RECS_ON_PAGE катаологов
//static int firstDir = 0;
/// Номер подсвеченного каталога
//static int curDir = 0;
/// Номер первого выведенного файла. Всего может быть выведено RECS_ON_PAGE файлов
//static int firstFile = 0;
/// Номер подсвеченного итема
static uint curItem = 0;
/// true, если флешка подключена
static bool isConnected = false;

static uint numDirs = 0;

static uint numFiles = 0;
/// Путь к текущему каталогу
static char directory[255] = "\\";
/// Здесь хранятся имена каталогов или файлов
static const int NUM_ITEMS = 20;
static char names[NUM_ITEMS][50];

static bool isBusy = false;

/// В каком состоянии находится обмен с флешкой
struct State
{
    enum E
    {
        NeedRepaint,            ///< Нужна перерисовка
        Idle,                   ///< Ничего делать не нужно
        Wait                    ///< Ожидание ответа Interface
    } value;
};

static State::E state = State::NeedRepaint;

FDrive::View FDrive::view = FDrive::Files;
/// Здесь хранится сообщение от устройства
static uint8 message[300] = {0};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Выводит название текущего каталога в координатах [left, top]
static void DrawNameCurrentDir(int left, int top);
/// Обработка сообщения
static void ProcessMessage();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FDrive::Graphics::Init()
{
    strcpy(directory, "\\");
    //firstDir = firstFile = curDir = curFile = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Graphics::Draw()
{
    DEBUG_POINT;
    ProcessMessage();
    DEBUG_POINT;

    int x = Wave::Graphics::X();
    int y = Wave::Graphics::Y(Chan::A) + 1;
    int width = Wave::Graphics::Width() - 2;
    int height = Wave::Graphics::Height() * 2;
    Painter::FillRegion(x, y, width, height, Color::BACK);
    DEBUG_POINT;
    if(!isConnected)
    {
        Text::DrawBigText(30, 110, 2, "Подключите флешку", Color::FILL);
        return;
    }

    DrawNameCurrentDir(x + 3, y + 1);

    if(state == State::NeedRepaint)
    {
        for(int i = 0; i < NUM_ITEMS; i++)
        {
            names[i][0] = '\0';
        }

        numDirs = numFiles = 0;

        FDrive::RequestNumDirsAndFiles(directory);
        state = State::Wait;
    }
    DEBUG_POINT
    if(view == Dirs)
    {
        DrawDirs();
    }
    else
    {
        DrawFiles();
    }
    DEBUG_POINT;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawNameCurrentDir(int left, int top)
{
    Text::DrawText(left, top, directory, Color::FILL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::HandlerInterface(uint8 *data)
{
    if(message[0])
    {
        LOG_WRITE("Не обработано предыдущее сообщение");
        Console::Draw();
        Painter::EndScene();
        return;
    }

    if(isBusy)
    {
        LOG_WRITE("Я занят %d", *data);
        return;
    }
    memcpy(message, data, 300);
    ProcessMessage();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ProcessMessage()
{
    isBusy = true;

    uint8 *data = &message[0];

    if(*data == 0)
    {
        isBusy = false;
        return;
    }

    if(*data == Command::FDrive_Mount)
    {
        isConnected = *(data + 1) != 0;
        if(!isConnected)
        {
            state = State::NeedRepaint;
        }
    }
    else if(*data == Command::FDrive_NumDirsAndFiles)
    {
        BitSet32 dirs(data + 1);
        BitSet32 files(data + 5);

        numDirs = dirs.word;
        numFiles = files.word;

        state = State::Idle;
    }
    else if(*data == Command::FDrive_RequestDir || *data == Command::FDrive_RequestFile)
    {
        BitSet32 num(data + 1);

        char *src = (char *)(data + 5);
        char *dest = &names[num.word][0];

        while(*src)
        {
            *dest++ = *src++;
        }

        *dest = '\0';

        state = State::Idle;
    }
    else
    {
        LOG_WRITE("Не понимаю сообщения");
    }

    message[0] = 0;

    isBusy = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::DrawDirs()
{
    DEBUG_POINT;
    int x = Wave::Graphics::X() + 5;
    int y = Wave::Graphics::Y(Chan::A) + 15;
    int delta = 10;

    for(uint i = 0; i < numDirs; i++)
    {
        Text::DrawText(x, y, names[i], Color::FILL);
        y += delta;
    }
    DEBUG_POINT;
    if(state == State::Idle)
    {
        for(uint i = 0; i < numDirs; i++)
        {
            if(names[i][0] == 0)
            {
                FDrive::RequestNameDir(i, directory);
                state = State::Wait;
                break;
            }
        }
    }
    DEBUG_POINT;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::DrawFiles()
{
    DEBUG_POINT;
    int x = Wave::Graphics::X() + 5;
    int y = Wave::Graphics::Y(Chan::A) + 15;
    int delta = 10;

    for(uint i = 0; i < numFiles; i++)
    {
        DEBUG_POINT;
        if(names[i])
        {
            DEBUG_POINT;
            names[i][11] = '\0';
            DEBUG_POINT
            bool selected = (i == curItem);                     // Признак того, что данный итем является подсвеченным
            DEBUG_POINT;
            if(selected)
            {
                DEBUG_POINT;
                Painter::FillRegion(x - 1, y, 100, 9, Color::FILL);
                DEBUG_POINT
            }
            DEBUG_POINT;
            Text::DrawText(x, y, names[i], (selected) ? Color::BACK : Color::FILL);
            DEBUG_POINT;
        }
        y += delta;
    }
    DEBUG_POINT;
    if(state == State::Idle)
    {
        for(uint i = 0; i < numFiles; i++)
        {
            if(names[i][0] == 0)
            {
                FDrive::RequestNameFile(i, directory);
                state = State::Wait;
                break;
            }
        }
    }
    DEBUG_POINT;
}
