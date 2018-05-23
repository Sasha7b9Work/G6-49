#include "defines.h"
#include "Settings.h"
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Generator.h"
#include "Hardware/CPU.h"



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
    }
    ,
    A,                                  // sig_channel
    { Sine, Sine },                     // sig_form[NumChannels]
    { 
        Frequency,                      // sig_parameter[NumForms]
        Frequency,
        Frequency
    },
                                        // sig_structParameter
    {
        {
            {                                                                           // [A][Form_Sine]
                {"20000", "", 0, 0, NUM_DIGITS - 1, Kilo,  Frequency, Sign_None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Milli, Period,    Sign_None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, One,   Amplitude, Sign_None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, One,   Offset,    Sign_Plus, 1, 0},
                {},
                {},
                {},
                {}
            },
            {                                                                           // [A][Form_Saw]
                {"10000", "", 0, 0, NUM_DIGITS - 1, Kilo,  Frequency, Sign_None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Milli, Period,    Sign_None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, One,   Amplitude, Sign_None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, One,   Offset,    Sign_Plus, 1, 0},
                {},
                {},
                {},
                {}
            },
            {                                                                           // [A][Form_Impulse]
                {"20000", "", 0, 0, NUM_DIGITS - 1, Kilo,  Frequency, Sign_None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Milli, Period,    Sign_None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, One,   Amplitude, Sign_None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, One,   Offset,    Sign_Plus, 1, 0},
                {"10000", "", 2, 0, NUM_DIGITS - 1, Micro, Duration,  Sign_None, 1, 0},
                {"20000", "", 0, 0, NUM_DIGITS - 1, One,   DutyRatio, Sign_None, 1, 0},
                {},
                {}
            }    
                
        },
        {
            {                                                                           // [B][Form_Sine]
                {"20001", "", 0, 0, NUM_DIGITS - 1, Kilo,  Frequency, Sign_None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Milli, Period,    Sign_None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, One,   Amplitude, Sign_None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, One,   Offset,    Sign_Plus, 1, 0},
                {},
                {},
                {"10000", "", 0, 0, NUM_DIGITS - 1, One,   Phase,     Sign_Plus, 1, 0},
                {}
            },
            {                                                                           // [B][Form_Saw]
                {"10000", "", 0, 0, NUM_DIGITS - 1, Kilo,  Frequency, Sign_None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Milli, Period,    Sign_None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, One,   Amplitude, Sign_None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, One,   Offset,    Sign_Plus, 1, 0},
                {},
                {},
                {},
                {}
            },
            {                                                                           // [B][Form_Impulse]
                {"20000", "", 0, 0, NUM_DIGITS - 1, Kilo,  Frequency, Sign_None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Milli, Period,    Sign_None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, One,   Amplitude, Sign_None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, One,   Offset,    Sign_Plus, 1, 0},
                {"10000", "", 2, 0, NUM_DIGITS - 1, Micro, Duration,  Sign_None, 1, 0},
                {"20000", "", 0, 0, NUM_DIGITS - 1, One,   DutyRatio, Sign_None, 1, 0},
                {},
                {"50000", "", 1, 0, NUM_DIGITS - 1, Micro, Delay,     Sign_None, 1, 0}
            } 
        }
    },
    RU,              // serv_language
    0,               // menu_currentPage
    (Page*)0,        // menu_page
    true,            // dbg_console
    {true, true},    // sig_enabled
    true,            // serv_backgroundBlack
    true,            // sig_tuneFull
    false,           // dbg_debugModeEnabled
    {0},             // menu_posActItem[]
    {0},             // menu_currentSubPage[]
    false,           // freq_enabled
    SizeByte_8bit,   // usb_sizeByte
    StopBit_1,       // usb_stopBit
    Parity_Off,      // usb_parity
    Interval_1,      // freq_interval
    BillingTime_1ms, // freq_billingTime
    false             // dbg_statistics
};

Settings set = defSet;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const float minMax[NumParameters][2] =
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
float MinValue(WaveParameter param)
{
    return minMax[param][0];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float MaxValue(WaveParameter param)
{
    return minMax[param][1];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Save(void)
{
    CPU::FLASH_::SaveSettings();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Load(bool _default)
{
    set = defSet;

    /*
    if(_default)
    {
        set = defSet;
    }
    else
    {
        CPU::FLASH_::LoadSettings();
    }
    */

    TuneGenerator(A);
    TuneGenerator(B);
    
    Painter::LoadPalette();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void TuneGenerator(Channel ch)
{
    Generator::EnableChannel(ch, set.sig_enabled[ch]);
    WaveForm form = WAVE_FORM_CH(ch);
    Generator::SetFormWave(ch, WAVE_FORM_CH(ch));
    Generator::SetParameter(ch, Frequency, (&INPUT_WINDOW_STRUCT(ch, form, Frequency))->Value());
    Generator::SetParameter(ch, Amplitude, (&INPUT_WINDOW_STRUCT(ch, form, Amplitude))->Value());
    if (form.type == Impulse)
    {
        Generator::SetParameter(ch, Duration, (&INPUT_WINDOW_STRUCT(ch, form, Duration))->Value());
    }
    Generator::SetParameter(ch, Offset,    (&INPUT_WINDOW_STRUCT(ch, WAVE_FORM_CH(ch), Offset))->Value());
}
