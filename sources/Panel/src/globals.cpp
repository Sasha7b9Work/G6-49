#include "globals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputWindow::Struct sig_structParameter[Chan::Number][Wave::Form::Number][Wave::Parameter::Number] =
{
            {
            {                                                                           // [A][Form::Sine]
/* Frequency */ {' ', "20000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0, 0.1f, 30000000.0f},
/* Period    */ {' ', "05000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0, 3.3333333333e-8f},
/* Amplitude */ {' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0, 0.0f, 10.0f},
/* Offset    */ {'+', "20480", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Offset,    1, 0, -5.0f, 5.0f},
/* Duration  */ {},
/* DutyRatio */ {},
/* Phase     */ {},
/* Delay     */ {},
///* DepthMod  */ {"00000", "", 0, 0, NUM_DIGITS - 1, One,  DepthModulation, Sign_None, 1, 0},
/* Polarity  */ {},
/* DurRise   */ {},
/* DurFall   */ {},
/* DurStady  */ {},
/* DurFactor */ {}
            },
            {                                                                           // [A][Form::RampPlus]
                {' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0},
                {' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0},
                {' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0},
                {'+', "00000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    1, 0},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {}
            },
            {                                                                           // [A][Form::RampMinus]
                {' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0},
                {' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0},
                {' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0},
                {'+', "00000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    1, 0},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {}
            }

        },
        {
            {                                                                           // [B][Form::Sine]
                {' ', "20001", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0},
                {' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0},
                {' ', "40960", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Amplitude, 1, 0},
                {'+', "20480", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Offset,    1, 0},
                {},
                {},
                {'+', "10000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Phase,     1, 0},
                {}
            },
            {                                                                           // [B][Form::RampPlus]
                {' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0},
                {' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0},
                {' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0},
                {'+', "00000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    1, 0},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {}
            },
            {                                                                           // [B][Form::RampMinus]
                {' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0},
                {' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0},
                {' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0},
                {'+', "00000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    1, 0},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {}
            }
        }
};
