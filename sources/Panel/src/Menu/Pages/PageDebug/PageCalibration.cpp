#include "defines.h"
#include "common/Message_p.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Menu/Pages/PageDebug/PageDebug.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"


extern const PageBase pCalibration;


static uint8 channel = 0;
static uint8 signal = 0;
static uint8 parameter = 0;
static uint8 range = 0;

static int16 *calK = nullptr;


static void LoadK()
{
    calK = setCal.GetK(channel, signal, range, parameter);
}

static uint8 prevChannel = 255;
static uint8 prevSignal = 255;
static uint8 prevRange = 255;
static uint8 prevParameter = 255;
static int16 *prevPointerK = nullptr;
static int16 prevK = 0;


static void SendMessage()
{
    if(channel != prevChannel || prevSignal != signal || prevRange != range || prevParameter != parameter || prevPointerK != calK || prevK != *calK)
    {
        prevChannel = channel;
        prevSignal = signal;
        prevRange = range;
        prevParameter = parameter;
        prevPointerK = calK;
        prevK = *calK;

        Message::CalibrateSet(channel, signal, range, parameter).Transmit();
        setCal.Save();
    }
}


/// Вызывается при изменении калибруемого параметра
static void OnChange_Parameters(bool)
{
    LoadK();
    SendMessage();
}

/// Вызывается при изменении источника сигнал
static void OnChange_Source(bool)
{
    LoadK();
    OnChange_Parameters(true);
    SendMessage();
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

DEF_CHOICE_5(cRange,
    "Диапазон",
    "",
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
    Painter::FillRegion(10, 10, 200, 50, Color::WHITE);

    char buffer[30];

    SU::Int2String(*calK, true, 1, buffer);

    Text::DrawBigText(20, 20, 3, buffer, Color::BLACK);
}


static bool FuncOnKeyPage(const Key &key) //-V2009
{
    if(key.IsRotate())
    {
        if(key.action == Key::Action::Down)
        {
            if(key.value == Key::RegLeft)
            {
                setCal.ReduceK();
                SendMessage();
            }
            else
            {
                setCal.IncreaseK();
                SendMessage();
            }
        }

        return true;
    }

    return false;
}


static void OnEnter_Calibration(bool enter)
{
    if(enter)
    {
        LoadK();
        SendMessage();
    }
    else
    {
        set.Load();

        prevChannel = 255;
        prevSignal = 255;
        prevRange = 255;
        prevParameter = 255;
        prevPointerK = nullptr;
        prevK = 0;
    }
}


DEF_PAGE_4( pCalibration,
    "КАЛИБРОВКА",
    "",
    &cChannel,
    &cSignal,
    &cRange,
    &cParameter,
    Page::Calibration, PageDebug::self, Item::FuncActive, OnEnter_Calibration, FuncOnKeyPage, DrawPage
)

Page *PageDebug::Calibartion::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pCalibration));
