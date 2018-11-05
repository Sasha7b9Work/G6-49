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

static int numDirs = 0;

static int numFiles = 0;
/// Путь к текущему каталогу
static char directory[255] = "\\";

/// В каком состоянии находится обмен с флешкой
struct State
{
    enum E
    {
        None,               ///< Начальное состояние после запуска
        WaitNumDirsAndFiles ///< Ожидание ответа на запрос о количестве каталогов и файлов в directory
    } value;
};

static State::E state = State::None;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Выводит название текущего каталога в координатах [left, top]
static void DrawNameCurrentDir(int left, int top);


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

    if(state == State::None)
    {
        FDrive::RequestNumDirsAndFiles(directory);
        state = State::WaitNumDirsAndFiles;
    }
    else
    {
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawNameCurrentDir(int left, int top)
{
    Text::DrawText(left, top, directory, Color::FILL);
}
