#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FDrivePanel.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Wave.h"
#include "Hardware/CPU.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RECS_ON_PAGE    10

/// Номер первого выведенного каталога. Всего может быть выведено RECS_ON_PAGE катаологов
static int firstDir = 0;
/// Номер подсвеченного каталога
static int curDir = 0;
/// Номер первого выведенного файла. Всего может быть выведено RECS_ON_PAGE файлов
static int firstFile = 0;
/// Номер подсвеченного файла
static int curFile = 0;

static uint numDirs = 0;

static uint numFiles = 0;
/// Путь к текущему каталогу
static char directory[255] = "\\";
/// Здесь хранятся имена каталогов или файлов
static const int NUM_ITEMS = 20;
static char names[NUM_ITEMS][50];

/// В каком состоянии находится обмен с флешкой
struct State
{
    enum E
    {
        NeedRepaint,            ///< Нужна перерисовка
        Idle                    ///< Ничего делать не нужно
    } value;
};

static State::E state = State::NeedRepaint;

FDrive::View FDrive::view = FDrive::Dirs;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Выводит название текущего каталога в координатах [left, top]
static void DrawNameCurrentDir(int left, int top);
/// Написать список каталогов
static void DrawDirs();
/// Написать список файлов
static void DrawFiles();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FDrive::Graphics::Init()
{
    strcpy(directory, "\\");
    firstDir = firstFile = curDir = curFile = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Graphics::Draw()
{
    int x = Wave::Graphics::X();
    int y = Wave::Graphics::Y(Chan::A) + 1;
    int width = Wave::Graphics::Width() - 2;
    int height = Wave::Graphics::Height() * 2;
    Painter::FillRegion(x, y, width, height, Color::BACK);

    if(!FDrive::IsConnected())
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
        state = State::Idle;
    }

    if(view == Dirs)
    {
        DrawDirs();
    }
    else
    {
        DrawFiles();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawNameCurrentDir(int left, int top)
{
    Text::DrawText(left, top, directory, Color::FILL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::HandlerSetNumDirsAndFiles(uint _numDirs, uint _numFiles)
{
    numDirs = _numDirs;
    numFiles = _numFiles;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDirs()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawFiles()
{

}
