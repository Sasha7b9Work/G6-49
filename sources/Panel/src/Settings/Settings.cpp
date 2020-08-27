#include "Settings.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"


static const Settings defSet =
{
    sizeof(Settings),
    {
#ifdef WIN32
        MAKE_COLOR(0x00, 0x00, 0x00),   // BLACK
        MAKE_COLOR(0xff, 0xff, 0xff),   // WHITE
        MAKE_COLOR(25,   25,   25),     // GRAY_10
        MAKE_COLOR(0x30, 0x30, 0x30),   // GRAY_25
        MAKE_COLOR(0x80, 0x80, 0x80),   // GRAY_50
        MAKE_COLOR(0xC0, 0xC0, 0xC0),   // GRAY_75
        MAKE_COLOR(0x00, 0x00, 0xff),   // BLUE
        MAKE_COLOR(0x00, 0x00, 25),     // BLUE_10
        MAKE_COLOR(0x00, 0x00, 0x40),   // BLUE_25
        MAKE_COLOR(0x00, 0x00, 0x80),   // BLUE_50
        MAKE_COLOR(0x00, 0x00, 0xc0),   // BLUE_75
        MAKE_COLOR(0x00, 0xff, 0x00),   // GREEN
        MAKE_COLOR(0x00, 13,   0x00),   // GREEN_5
        MAKE_COLOR(0x00, 25,   0x00),   // GREEN_10
        MAKE_COLOR(0x00, 0x50, 0x00),   // GREEN_25
        MAKE_COLOR(0x00, 0x80, 0x00),   // GREEN_50
        MAKE_COLOR(0x00, 0xc0, 0x00),   // GREEN_75
        MAKE_COLOR(0xff, 0x00, 0x00),   // RED
        MAKE_COLOR(0x40, 0x00, 0x00),   // RED_25
        MAKE_COLOR(0x80, 0x00, 0x00),   // RED_50
        MAKE_COLOR(0xc0, 0x00, 0x00),   // RED_75
        MAKE_COLOR(0x40, 0xFF, 0x40),   // CHAN_A
        MAKE_COLOR(0xA0, 0xA0, 0xFF)    // CHAN_B
#else
        MAKE_COLOR(0x00, 0x00, 0x00),   // BLACK
        MAKE_COLOR(0xff, 0xff, 0xff),   // WHITE
        MAKE_COLOR(25,   25,   25),     // GRAY_10
        MAKE_COLOR(0x40, 0x40, 0x40),   // GRAY_25
        MAKE_COLOR(0x80, 0x80, 0x80),   // GRAY_50
        MAKE_COLOR(0xc0, 0xc0, 0xc0),   // GRAY_75
        MAKE_COLOR(0x00, 0x00, 0xff),   // BLUE
        MAKE_COLOR(0x00, 0x00, 25),     // BLUE_10
        MAKE_COLOR(0x00, 0x00, 0x40),   // BLUE_25
        MAKE_COLOR(0x00, 0x00, 0x80),   // BLUE_50
        MAKE_COLOR(0x00, 0x00, 0xc0),   // BLUE_75
        MAKE_COLOR(0x00, 0xff, 0x00),   // GREEN
        MAKE_COLOR(0x00, 13,   0x00),   // GREEN_5
        MAKE_COLOR(0x00, 25,   0x00),   // GREEN_10
        MAKE_COLOR(0x00, 0x40, 0x00),   // GREEN_25
        MAKE_COLOR(0x00, 0x80, 0x00),   // GREEN_50
        MAKE_COLOR(0x00, 0xc0, 0x00),   // GREEN_75
        MAKE_COLOR(0xff, 0x00, 0x00),   // RED
        MAKE_COLOR(0x40, 0x00, 0x00),   // RED_25
        MAKE_COLOR(0x80, 0x00, 0x00),   // RED_50
        MAKE_COLOR(0xc0, 0x00, 0x00),   // RED_75
        MAKE_COLOR(0x40, 0xFF, 0x40),   // CHAN_A
        MAKE_COLOR(0x40, 0x40, 0xFF)    // CHAN_B
#endif
    },
    static_cast<Page *>(PageMain::self), // menu_currentPage
    {0},                                    // menu_posActItem[]
    {0},                                    // menu_currentSubPage[]
    {false, false},                         // enabled[2]
    Chan::A,                                // current
    0,                                      // lang
    true                                    // bigSymbols
    ,                               
    {
        PFreqMeter::BillingTime::_1ms,      // freq_billingTime
        PFreqMeter::AvePeriod::_1,          // freq_avePeriod
        FreqTimeStamps::_10MHz,             // freq_timeStamps
        FreqMeasure::Disable,               // freq_measure
        0,                                  // freq_level
        0,                                  // freq_hysteresis
        FreqInterval::_1,
        FreqResist::_1MOhm,
        FreqCouple::AC,
        FreqFiltr::Disable,
        FreqTest::Off
    }
    ,
    {
        0, 0, 0, 0, 0, 0
    }
    ,
    {
        false,                              // showConsole
        0,                                  // dbgModeEnabled
        false,                              // showStatistics
        false                               // showSends
    }
};


Settings set = defSet;

void Settings::LoadDefault()
{
    set = defSet;
}


void Settings::LoadToDevice()
{
    PGenerator::LoadSettings();

    PFreqMeter::LoadSettings();

    Painter::LoadPalette();
}


Settings& Settings::operator=(const Settings &rhs)
{
    if(this != &rhs)
    {
        std::memcpy(this, &rhs, sizeof(Settings));
    }
    return *this;
}
