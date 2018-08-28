#include "defines.h"
#include "globals.h"
#include "Settings.h"
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Generator/Generator.h"
#include "Hardware/CPU.h"


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Settings defSet =
{
    sizeof(Settings),
    {
    },
    {
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
        MAKE_COLOR(0xc0, 0x00, 0x00)    // RED_75
    },
    { Wave::Form::Sine, Wave::Form::Sine }, // sig_form[NumChannels]
    { 
        Wave::Parameter::Frequency,                      // sig_parameter[NumForms]
        Wave::Parameter::Frequency,
        Wave::Parameter::Frequency,
//        Wave::Parameter::Frequency,
//        Wave::Parameter::Frequency,
//        Wave::Parameter::Frequency,
//        Wave::Parameter::Frequency,
//        Wave::Parameter::Frequency,
//        Wave::Parameter::Frequency,
//        Wave::Parameter::Frequency,
//        Wave::Parameter::Frequency,
//        Wave::Parameter::Frequency
    },
    3,                      // menu_currentPage
    (Page*)0,               // menu_page
    {0},                    // menu_posActItem[]
    {0},                    // menu_currentSubPage[]
    BillingTime::_1ms,      // freq_billingTime
    FreqAvePeriod::_1,      // freq_avePeriod
    FreqTimeStamps::_10MHz, // freq_timeStamps

    BINARY_U8(01001100), // flag1
//            ||||||||------ 0 BIT_PARITY         Parity_Off
//            |||||||------- 1 BIT_FREQ_INTERVAL  Interval_1
//            ||||||-------- 2 BIT_CONSOLE      
//            |||||--------- 3 BIT_BACK_BLACK   
//            ||||---------- 4 BIT_TUNE_FULL    
//            |||----------- 5 BIT_DBG_MODE     
//            ||------------ 6 BIT_FREQ_ENABLED 
//            |------------- 7 BIT_STATISTICS   

    BINARY_U8(00010110), // flag2
//            ||||||||------ 0 BIT_LANGUAGE       RU
//            |||||||------- 1 BIT_CHAN_A   
//            ||||||-------- 2 BIT_CHAN_B   
//            |||||--------- 3 BIT_CHANNEL        A
//            ||||---------- 4 BIT_SIZE_BYTE      8bit
//            |||----------- 5 BIT_STOP_BIT       StopBit_1
//            ||------------ 6 BIT_FREQ_RESIST
//            |------------- 7 BIT_FREQ_COUPLE

    BINARY_U8(00000000) // flag3
//                  ||------ 0 BIT_FREQ_FILTR
//                  |------- 1 BIT_FREQ_MEASURE
};

#ifdef WIN32
#pragma warning(pop)
#endif

Settings set = defSet;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const float minMax[Wave::Parameter::Number][2] =
{
    {1.0f,    50e6f},       // Frequency
    {2e-08f,  1.0f},        // Period
    {0.000f,  65535.0f},    // Amplitude
    {0.0f,    4095.0f},     // Offset
    {2e-08f,  10.0f},       // Duration
    {0.001f,  1.0f},        // DutyRatio
    {-180.0f, 180.0f},      // Phase
    {2e-08f,  10e08f}       // Delay
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float Wave::Parameter::MinValue() const
{
    return minMax[value][0];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Wave::Parameter::MaxValue() const
{
    return minMax[value][1];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Save()
{
    CPU::FLASH_::SaveSettings();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Load(bool _default)
{
    set = defSet;

    if(_default)
    {
        set = defSet;
    }
    else
    {
        CPU::FLASH_::LoadSettings();
    }

    TuneGenerator(Chan::A);
    TuneGenerator(Chan::B);
    
    Painter::LoadPalette();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::TuneGenerator(Chan ch)
{
    Generator::EnableChannel(ch, CHANNEL_ENABLED(ch));
    Wave::Form form = WAVE_FORM(ch);
    Generator::SetFormWave(ch, WAVE_FORM(ch));
    Generator::SetParameter(ch, Wave::Parameter::Frequency, (&INPUT_WINDOW_STRUCT(ch, form, Wave::Parameter::Frequency))->Value());
    Generator::SetParameter(ch, Wave::Parameter::Amplitude, (&INPUT_WINDOW_STRUCT(ch, form, Wave::Parameter::Amplitude))->Value());
    Generator::SetParameter(ch, Wave::Parameter::Offset,    (&INPUT_WINDOW_STRUCT(ch, form, Wave::Parameter::Offset))->Value());
    Generator::SetParameter(ch, Wave::Parameter::Offset,    (&INPUT_WINDOW_STRUCT(ch, WAVE_FORM(ch), Wave::Parameter::Offset))->Value());
}
