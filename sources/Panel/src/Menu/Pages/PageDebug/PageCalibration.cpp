#include "defines.h"
#include "Menu/Pages/PageDebug/PageDebug.h"


extern const PageBase pCalibration;


static uint flag = 0;
#define FLAG_K flag
#define B_CHANNEL   0
#define B_SIGNAL    1
#define B_PARAMETER 2

static uint8 parameter = 0;
static uint8 range = 0;


/// Вызывается при изменении параметров калибровки
static void OnChange_Parameters(bool)
{

}


DEF_CHOICE_2(cChannel,
    "КАНАЛ",
    "",
    "A", "",
    "B", "",
    FLAG_K, B_CHANNEL, pCalibration, Item::FuncActive, OnChange_Parameters, FuncDraw
)

DEF_CHOICE_2(cSignal,
    "СИГНАЛ",
    "",
    "Синус", "",
    "DDS", "",
    FLAG_K, B_SIGNAL, pCalibration, Item::FuncActive, OnChange_Parameters, FuncDraw
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


DEF_PAGE_6( pCalibration,
    "КАЛИБРОВКА",
    "",
    &cChannel,
    &cSignal,
    &cRange,
    &cParameter,
    PageDebug::_Calibration::PageA::self, ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A
    PageDebug::_Calibration::PageB::self, ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B
    Page::Calibration, PageDebug::self, Item::FuncActive, FuncPress, FuncOnKey, Page::FuncDraw
)

Page *PageDebug::Calibartion::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pCalibration));
