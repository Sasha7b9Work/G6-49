#include "defines.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Menu/Pages/PageDebug/PageDebug.h"
#include "Menu/Pages/PageDebug/StructCalibration.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"


extern const PageBase pCalibration;


static uint8 channel = 0;
static uint8 signal = 0;
static uint8 parameter = 0;
static uint8 range = 0;

static int16 *k = nullptr;


void PageDebug::Calibartion::Init()
{
    k = StructCalibration::GetK(channel, signal, range, parameter);
}

/// Вызывается при изменении калибруемого параметра
static void OnChange_Parameters(bool)
{

}


/// Вызывается при изменении источника сигнал
static void OnChange_Source(bool)
{
    k = StructCalibration::GetK(channel, signal, range, parameter);
    OnChange_Parameters(true);
}

DEF_CHOICE_3(cChannel,
    "КАНАЛ",
    "",
    "A", "",
    "B", "",
    "", "",
    channel, pCalibration, Item::FuncActive, OnChange_Source, FuncDraw
)

DEF_CHOICE_3(cSignal,
    "СИГНАЛ",
    "",
    "Синус", "",
    "DDS", "",
    "", "",
    signal, pCalibration, Item::FuncActive, OnChange_Source, FuncDraw
)

DEF_CHOICE_6(cRange,
    "Диапазон",
    "",
    "30 мВ", "",
    "100 мВ", "",
    "300 мВ", "",
    "1 В", "",
    "3 В", "",
    "10 В", "",
    range, pCalibration, Item::FuncActive, OnChange_Parameters, FuncDraw
)

DEF_CHOICE_4(cParameter,
    "Параметр",
    "",
    "Амплитуда", "",
    "+5В", "",
    "0В", "",
    "-5В", "",
    parameter, pCalibration, Item::FuncActive, OnChange_Parameters, FuncDraw
)


static void DrawPage()
{
    Painter::FillRegion(10, 10, 100, 100, Color::WHITE);

    char buffer[30];

    SU::Int2String(*k, true, 1, buffer);

    Text::Draw(20, 20, buffer, Color::BLACK);
}


static bool FuncOnKeyPage(Key &key)
{
    if(key.IsRotate())
    {
        return true;
    }

    return false;
}


DEF_PAGE_6( pCalibration,
    "КАЛИБРОВКА",
    "",
    &cChannel,
    &cSignal,
    &cRange,
    &cParameter,
    PageDebug::_Calibration::PageA::self, ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A
    PageDebug::_Calibration::PageB::self, ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B
    Page::Calibration, PageDebug::self, Item::FuncActive, FuncPress, FuncOnKeyPage, DrawPage
)

Page *PageDebug::Calibartion::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pCalibration));
