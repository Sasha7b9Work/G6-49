#include "defines.h"
#include "common/Messages_p.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Menu/Pages/PageDebug/PageDebug.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"


extern const PageBase pCalibration;


static uint8 channel = 0;
static uint8 signal = 0;
static uint8 parameter = 0;
static uint8 range = 0;

static int16 *calK = nullptr;

static uint8 prevChannel = 255;
static uint8 prevSignal = 255;
static uint8 prevRange = 255;
static uint8 prevParameter = 255;
static int16 *prevPointerK = nullptr;
static int16 prevK = 0;


/// Настроить органы управления в соотвествии с текущими установами
static void TuneControls();


static void LoadK()
{
    calK = setCal.GetK(channel, signal, range, parameter);
}


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
    TuneControls();
    LoadK();
    SendMessage();
}

/// Вызывается при изменении источника сигнал
static void OnChange_Source(bool)
{
    TuneControls();
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

/// Для смещения +/- 5В
DEF_CHOICE_4(cParameterFullVoltage,
    "Параметр",
    "",
    "Амплитуда", "",
    "+5В", "",
    "0В", "",
    "-5В", "",
    parameter, pCalibration, Item::FuncActive, OnChange_Parameters, FuncDraw
)


/// Для смещения +/- 2.5В
DEF_CHOICE_4(cParameterHalfVoltage,
    "Параметр",
    "",
    "Амплитуда", "",
    "+2.5В", "",
    "0В", "",
    "-2.5В", "",
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
    if(key.value == Key::RegButton && key.action == Key::Down)
    {
        Math::CircleIncrease<uint8>(&range, 0, 4);
        OnChange_Parameters(true);
        return true;
    }
    else if(key.IsRotate())
    {
        if(key.action == Key::Down)
        {
            int16 min = -1024;
            int16 max = 1024;

            if(parameter == 1)      { max = 0; }            // для смещения +max будем ограничивать нулём сверху - можно сделать только меньше
            else if(parameter == 3) { min = 0; }            // для смещения -max будем ограничивать нулём снизу - можно сделать только больше
            if(parameter == 0 && signal == 1) { max = 0; }  // для амплитуды произвольного сигнала будем ограничивать нулём сверху - можно сделать только меньше

            if(key.value == Key::RegLeft)
            {
                setCal.ReduceK(min);
                SendMessage();
            }
            else
            {
                setCal.IncreaseK(max);
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
        TuneControls();
        LoadK();
        SendMessage();
    }
    else
    {
        set.LoadToDevice();

        prevChannel = 255;
        prevSignal = 255;
        prevRange = 255;
        prevParameter = 255;
        prevPointerK = nullptr;
        prevK = 0;
    }
}


const PageBase *page = const_cast<const PageBase *>(reinterpret_cast<PageBase *>(PageDebug::self));


DEF_PAGE_4_VAR( pCalibration,
    "КАЛИБРОВКА",
    "",
    cChannel,
    cSignal,
    cRange,
    cParameterFullVoltage,
    Page::Calibration, page, Item::FuncActive, OnEnter_Calibration, FuncOnKeyPage, DrawPage
)

Page *PageDebug::Calibartion::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pCalibration));


static void TuneControls()
{
    if(range < 2)
    {
        pCalibration.items[3] = reinterpret_cast<Item *>(const_cast<ChoiceBase *>(&cParameterHalfVoltage));
    }
    else
    {
        pCalibration.items[3] = reinterpret_cast<Item *>(const_cast<ChoiceBase *>(&cParameterFullVoltage));
    }
}
