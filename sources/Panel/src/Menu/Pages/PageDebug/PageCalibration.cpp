// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Interface/Messages_p.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Menu/Pages/Pages.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"


using namespace Primitives;


extern const PageBase pCalibration;


static uint8 channel = 0;
static uint8 signal = 0;
static uint8 range = 0;
static uint8 parameter = 0;

static int16 *calK = nullptr;

static uint8 prevChannel = 255;
static uint8 prevSignal = 255;
static uint8 prevRange = 255;
static uint8 prevParameter = 255;
static int16 *prevPointerK = nullptr;
static int16 prevK = 0;


// Настроить органы управления в соотвествии с текущими установами
static void TuneControls();


static bool ChannelIsValid()
{
    return (channel == 0) || (channel == 1);
}


static void LoadK()
{
    if(!ChannelIsValid())
    {
        return;
    }

    calK = setCal.GetK(channel, signal, range, parameter);
}


static void SendMessage()
{
    if(!ChannelIsValid())
    {
        return;
    }

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


// Вызывается при изменении калибруемого параметра
static void OnChange_Parameters(bool active)
{
    if (active)
    {
        if (!ChannelIsValid())
        {
            return;
        }

        TuneControls();

        LoadK();

        SendMessage();
    }
}

// Вызывается при изменении источника сигнал
static void OnChange_Source(bool active)
{
    if (active)
    {
        if (!ChannelIsValid())
        {
            return;
        }

        TuneControls();
        LoadK();
        OnChange_Parameters(true);
        SendMessage();
    }
}

DEF_CHOICE_3(cChannelCal,
    "КАНАЛ", "CHANNEL",
    "", "",
    "A", "A", "", "",
    "B", "B", "", "",
    "",  "",  "", "",
    channel, pCalibration, Item::EFuncActive, OnChange_Source, FuncDraw
)

DEF_CHOICE_3(cSignal,
    "СИГНАЛ", "Signal",
    "", "",
    "Синус", "Sinus", "", "",
    "DDS", "DDS", "", "",
    "", "", "", "",
    signal, pCalibration, Item::EFuncActive, OnChange_Source, FuncDraw
)

DEF_CHOICE_5(cRange,
    "Диапазон", "Range",
    "", "",
    "100 мВ", "100 mV", "", "",
    "300 мВ", "300 mv", "", "",
    "1 В",    "1 V",    "", "",
    "3 В",    "3 V",    "", "",
    "10 В",   "10 V",   "", "",
    range, pCalibration, Item::EFuncActive, OnChange_Parameters, FuncDraw
)

// Для смещения +/- 5В
DEF_CHOICE_4(cParameterFullVoltage,
    "Параметр", "Param",
    "", "",
    "Размах",  "Amplitude", "", "",
    "+5В",     "+5V",       "", "",
    "0В",      "0V",        "", "",
    "-5В",     "-5V",       "", "",
    parameter, pCalibration, Item::EFuncActive, OnChange_Parameters, FuncDraw
)


// Для смещения +/- 2.5В
DEF_CHOICE_4(cParameterHalfVoltage,
    "Параметр", "Param",
    "", "",
    "Размах",  "Amplitude", "", "",
    "+2.5В",   "+2.5V",     "", "",
    "0В",      "0V",        "", "",
    "-2.5В",   "-2.5V",     "", "",
    parameter, pCalibration, Item::EFuncActive, OnChange_Parameters, FuncDraw
)


static void DrawPage()
{
    if(!ChannelIsValid())
    {
        return;
    }

    Rectangle(200, 50).Fill(10, 10, Color::WHITE);

    char buffer[30];

    SU::Int2String(*calK, true, 1, buffer);

    BigText(buffer, 3).Draw(20, 20, Color::BLACK);
}


static bool FuncOnControlKeyPage(const Control &control)
{
    if(control.Is(Key::RegButton, Action::Down))
    {
        Math::CircleIncrease<uint8>(&range, 0, 4);
        OnChange_Parameters(true);
        return true;
    }
    else if(control.IsRotate())
    {
        if(control.IsDown())
        {
            int16 min = -1024;
            int16 max = 1024;

            if(parameter == 1)      { max = 0; }    // для смещения +max будем ограничивать нулём сверху - можно сделать только меньше
            else if(parameter == 3) { min = 0; }    // для смещения -max будем ограничивать нулём снизу - можно сделать только больше
            if(parameter == 0)      { max = 0; }    // амплитуду можно только уменьшать

            if(control.Is(Key::RotateLeft))
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
    if (enter)
    {
        Message::Debug().Transmit();
    }

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

    if (!enter)
    {
        Message::Debug().Transmit();
    }
}


const PageBase *page = (const PageBase *)(PageBase *)PageDebug::self;


DEF_PAGE_4_VAR( pCalibration,
    "КАЛИБРОВКА", "CALIBRATION",
    "", "",
    &cChannelCal,
    &cSignal,
    &cRange,
    &cParameterFullVoltage,
    Page::Calibration, page, Item::EFuncActive, OnEnter_Calibration, FuncOnControlKeyPage, DrawPage
)


Page *PageDebug::Calibartion::self = (Page *)&pCalibration;


static void TuneControls()
{
    if(range < 3)
    {
        pCalibration.items[3] = (Item *)&cParameterHalfVoltage;
    }
    else
    {
        pCalibration.items[3] = (Item *)&cParameterFullVoltage;
    }
}

