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
        MAKE_COLOR(0x00, 0x00, 0x00),
        MAKE_COLOR(0xff, 0xff, 0xff),
        MAKE_COLOR(0xa0, 0xa0, 0xa0),
        MAKE_COLOR(0x00, 0x00, 0xff)
    }
    ,
    A,                              // sig_channel
    { Form_Sine, Form_Sine },       // sig_form[NumChannels]
    { 
        Frequency,    // sig_parameter[NumForms]
        Frequency,
        Frequency
    },
    {
        {
            {                                                                           // [A][Form_Sine]
                {true, Sign_None, "20000", "", {}, 0, 0, NUM_DIGITS - 1, Kilo,  Frequency, false, 0},
                {true, Sign_None, "10000", "", {}, 0, 0, NUM_DIGITS - 1, Milli, Period,    false, 0},
                {true, Sign_None, "50000", "", {}, 0, 0, NUM_DIGITS - 1, One,   Amplitude, false, 0},
                {true, Sign_Plus, "00000", "", {}, 0, 0, NUM_DIGITS - 1, One,   Offset,    false, 0},
                {},
                {},
                {},
                {}
            },
            {                                                                           // [A][Form_Saw]
                {true, Sign_None, "10000", "", {}, 0, 0, NUM_DIGITS - 1, Kilo,  Frequency, false, 0},
                {true, Sign_None, "10000", "", {}, 0, 0, NUM_DIGITS - 1, Milli, Period,    false, 0},
                {true, Sign_None, "50000", "", {}, 0, 0, NUM_DIGITS - 1, One,   Amplitude, false, 0},
                {true, Sign_Plus, "00000", "", {}, 0, 0, NUM_DIGITS - 1, One,   Offset,    false, 0},
                {},
                {},
                {},
                {}
            },
            {                                                                           // [A][Form_Impulse]
                {true, Sign_None, "20000", "", {}, 0, 0, NUM_DIGITS - 1, Kilo,  Frequency, false, 0},
                {true, Sign_None, "10000", "", {}, 0, 0, NUM_DIGITS - 1, Milli, Period,    false, 0},
                {true, Sign_None, "50000", "", {}, 0, 0, NUM_DIGITS - 1, One,   Amplitude, false, 0},
                {true, Sign_Plus, "00000", "", {}, 0, 0, NUM_DIGITS - 1, One,   Offset,    false, 0},
                {true, Sign_None, "10000", "", {}, 2, 0, NUM_DIGITS - 1, Micro, Duration,  false, 0},
                {true, Sign_None, "20000", "", {}, 0, 0, NUM_DIGITS - 1, One,   DutyRatio, false, 0},
                {},
                {}
            }    
                
        },
        {
            {                                                                           // [B][Form_Sine]
                {true, Sign_None, "20001", "", {}, 0, 0, NUM_DIGITS - 1, Kilo,  Frequency, false, 0},
                {true, Sign_None, "10000", "", {}, 0, 0, NUM_DIGITS - 1, Milli, Period,    false, 0},
                {true, Sign_None, "50000", "", {}, 0, 0, NUM_DIGITS - 1, One,   Amplitude, false, 0},
                {true, Sign_Plus, "00000", "", {}, 0, 0, NUM_DIGITS - 1, One,   Offset,    false, 0},
                {},
                {},
                {true, Sign_Plus, "10000", "", {}, 0, 0, NUM_DIGITS - 1, One,   Phase,     false, 0},
                {}
            },
            {                                                                           // [B][Form_Saw]
                {true, Sign_None, "10000", "", {}, 0, 0, NUM_DIGITS - 1, Kilo,  Frequency, false, 0},
                {true, Sign_None, "10000", "", {}, 0, 0, NUM_DIGITS - 1, Milli, Period,    false, 0},
                {true, Sign_None, "50000", "", {}, 0, 0, NUM_DIGITS - 1, One,   Amplitude, false, 0},
                {true, Sign_Plus, "00000", "", {}, 0, 0, NUM_DIGITS - 1, One,   Offset,    false, 0},
                {},
                {},
                {},
                {}
            },
            {                                                                           // [B][Form_Impulse]
                {true, Sign_None, "20000", "", {}, 0, 0, NUM_DIGITS - 1, Kilo,  Frequency, false, 0},
                {true, Sign_None, "10000", "", {}, 0, 0, NUM_DIGITS - 1, Milli, Period,    false, 0},
                {true, Sign_None, "50000", "", {}, 0, 0, NUM_DIGITS - 1, One,   Amplitude, false, 0},
                {true, Sign_Plus, "00000", "", {}, 0, 0, NUM_DIGITS - 1, One,   Offset,    false, 0},
                {true, Sign_None, "10000", "", {}, 2, 0, NUM_DIGITS - 1, Micro, Duration,  false, 0},
                {true, Sign_None, "20000", "", {}, 0, 0, NUM_DIGITS - 1, One,   DutyRatio, false, 0},
                {},
                {true, Sign_None, "50000", "", {}, 1, 0, NUM_DIGITS - 1, Micro, Delay,     false, 0}
            } 
        }
    },
    Russian,                        // serv_language
    0,                              // menu_currentPage
    (Control *)0,                   // menu_openedItem
    (Page*)0,                       // menu_page
    false,                          // dbg_console
    {true, true},                   // sig_enabled
    true,                           // serv_backgroundBlack
    true,                           // sig_tuneFull
    false                           // dbg_debugModeEnabled
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
    if(_default)
    {
        set = defSet;
    }
    else
    {
        CPU::FLASH_::LoadSettings();
    }

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
    if (form == Form_Impulse)
    {
        Generator::SetParameter(ch, Duration, (&INPUT_WINDOW_STRUCT(ch, form, Duration))->Value());
    }
    Generator::SetParameter(ch, Offset,    (&INPUT_WINDOW_STRUCT(ch, WAVE_FORM_CH(ch), Offset))->Value());
}
