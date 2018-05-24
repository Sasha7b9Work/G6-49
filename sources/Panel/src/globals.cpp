#include "globals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputWindowStruct sig_structParameter[NumChannels][NumForms][NumParameters] =
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
                {"20000", "", 0, 0, NUM_DIGITS - 1, One,   DutyCycle, Sign_None, 1, 0},
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
                {"20000", "", 0, 0, NUM_DIGITS - 1, One,   DutyCycle, Sign_None, 1, 0},
                {},
                {"50000", "", 1, 0, NUM_DIGITS - 1, Micro, Delay,     Sign_None, 1, 0}
            }
        }
};
