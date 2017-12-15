#include "defines.h"
#include "Settings.h"
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Generator/Generator.h"
#include "Hardware/FLASH.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Settings set =
{
    sizeof(Settings),
    {
        MAKE_COLOR(0, 0, 0),
        MAKE_COLOR(31, 63, 31),
        MAKE_COLOR(5, 10, 5)
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
                {true, Sign_None, "20000", "", 0, 0, NUM_DIGITS - 1, Kilo,  Frequency},
                {true, Sign_None, "10000", "", 0, 0, NUM_DIGITS - 1, Milli, Period},
                {true, Sign_None, "50000", "", 0, 0, NUM_DIGITS - 1, One,   Amplitude},
                {true, Sign_Plus, "00000", "", 0, 0, NUM_DIGITS - 1, One,   Offset},
                {false},
                {false},
                {false},
                {false}
            },
            {                                                                           // [A][Form_Saw]
                {true, Sign_None, "10000", "", 0, 0, NUM_DIGITS - 1, Kilo,  Frequency},
                {true, Sign_None, "10000", "", 0, 0, NUM_DIGITS - 1, Milli, Period},
                {true, Sign_None, "50000", "", 0, 0, NUM_DIGITS - 1, One,   Amplitude},
                {true, Sign_Plus, "00000", "", 0, 0, NUM_DIGITS - 1, One,   Offset},
                {false},
                {false},
                {false},
                {false}
            },
            {                                                                           // [A][Form_Impulse]
                {true, Sign_None, "20000", "", 0, 0, NUM_DIGITS - 1, Kilo,  Frequency},
                {true, Sign_None, "10000", "", 0, 0, NUM_DIGITS - 1, Milli, Period},
                {true, Sign_None, "50000", "", 0, 0, NUM_DIGITS - 1, One,   Amplitude},
                {true, Sign_Plus, "00000", "", 0, 0, NUM_DIGITS - 1, One,   Offset},
                {true, Sign_None, "10000", "", 2, 0, NUM_DIGITS - 1, Micro, Duration},
                {true, Sign_None, "20000", "", 0, 0, NUM_DIGITS - 1, One,   DutyRatio},
                {false},
                {false}
            }    
                
        },
        {
            {                                                                           // [B][Form_Sine]
                {true, Sign_None, "20001", "", 0, 0, NUM_DIGITS - 1, Kilo,  Frequency},
                {true, Sign_None, "10000", "", 0, 0, NUM_DIGITS - 1, Milli, Period},
                {true, Sign_None, "50000", "", 0, 0, NUM_DIGITS - 1, One,   Amplitude},
                {true, Sign_Plus, "00000", "", 0, 0, NUM_DIGITS - 1, One,   Offset},
                {false},
                {false},
                {true, Sign_Plus, "10000", "", 0, 0, NUM_DIGITS - 1, One,   Phase},
                {false}
            },
            {                                                                           // [B][Form_Saw]
                {true, Sign_None, "10000", "", 0, 0, NUM_DIGITS - 1, Kilo,  Frequency},
                {true, Sign_None, "10000", "", 0, 0, NUM_DIGITS - 1, Milli, Period},
                {true, Sign_None, "50000", "", 0, 0, NUM_DIGITS - 1, One,   Amplitude},
                {true, Sign_Plus, "00000", "", 0, 0, NUM_DIGITS - 1, One,   Offset},
                {false},
                {false},
                {false},
                {false}
            },
            {                                                                           // [B][Form_Impulse]
                {true, Sign_None, "20000", "", 0, 0, NUM_DIGITS - 1, Kilo,  Frequency},
                {true, Sign_None, "10000", "", 0, 0, NUM_DIGITS - 1, Milli, Period},
                {true, Sign_None, "50000", "", 0, 0, NUM_DIGITS - 1, One,   Amplitude},
                {true, Sign_Plus, "00000", "", 0, 0, NUM_DIGITS - 1, One,   Offset},
                {true, Sign_None, "10000", "", 2, 0, NUM_DIGITS - 1, Micro, Duration},
                {true, Sign_None, "20000", "", 0, 0, NUM_DIGITS - 1, One,   DutyRatio},
                {false},
                {true, Sign_None, "50000", "", 1, 0, NUM_DIGITS - 1, Micro, Delay}
            } 
        }
    },
    Russian,                        // serv_language
    0,                              // menu_currentPage
    (void*)0,                       // menu_openedItem
    (Page*)0,                       // menu_page
    false,                          // dbg_console
    {true, true},                   // sig_enabled
    false,                          // serv_colorBackground
    true,                           // sig_tuneFull
    false                           // dbg_debugModeEnabled
};


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
void Settings_Save(void)
{
    FLASH_SaveSettings();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Settings_Load(void)
{
    FLASH_LoadSettins();

    TuneGenerator(A);
    TuneGenerator(B);
    Display_SetColorBackground();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void TuneGenerator(Channel ch)
{
    Generator_EnableChannel(ch, set.sig_enabled[ch]);
    WaveForm form = WAVE_FORM_CH(ch);
    Generator_SetFormWave(ch, WAVE_FORM_CH(ch));
    Generator_SetParameter(ch, Frequency, IWS_Value(&INPUT_WINDOW_STRUCT(ch, form, Frequency)));
    Generator_SetParameter(ch, Amplitude, IWS_Value(&INPUT_WINDOW_STRUCT(ch, form, Amplitude)));
    if (form == Form_Impulse)
    {
        Generator_SetParameter(ch, Duration, IWS_Value(&INPUT_WINDOW_STRUCT(ch, form, Duration)));
    }
    Generator_SetParameter(ch, Offset,    IWS_Value(&INPUT_WINDOW_STRUCT(ch, WAVE_FORM_CH(ch), Offset)));
}
