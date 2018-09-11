#include "globals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputWindow::Struct sig_structParameter[Chan::Number][Wave::Form::Number][Wave::Parameter::Number] =
{
        {
            {                                                                           // [A][Form::Sine]
/* Frequency  */ InputWindow::Struct(' ', "20000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0, 0.1f, 100e6f),
/* Period     */ InputWindow::Struct(' ', "05000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0, 3.3333333333e-8f),
/* Amplitude  */ InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0, 0.0f, 10.0f),
/* Offset     */ InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    1, 0, 0.0f, 10.0f),
/* Duration   */ InputWindow::Struct(' '),
/* DutyRatio  */ InputWindow::Struct(' '),
/* Phase      */ InputWindow::Struct(' '),
/* Delay      */ InputWindow::Struct(' '),
/* Modul      */ InputWindow::Struct(' '),
/* Polarity   */ InputWindow::Struct(' '),
/* DurRise    */ InputWindow::Struct(' '),
/* DurFall    */ InputWindow::Struct(' '),
/* DurStady   */ InputWindow::Struct(' '),
/* DutFactor  */ InputWindow::Struct(' '),
/* Modulation */ InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::One,  Wave::Parameter::Modulation, 1, 0, 0.0f, 10.0f)
            },
            {                                                                           // [A][Form::RampPlus]
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0, 0.1f, 100e6f),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0, 3.33333e-8f, 10e6f),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            },
            {                                                                           // [A][Form::RampMinus]
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0, 0.1f, 100e6f),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0, 3.33333e-8f, 10e6f),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            },
            {                                                                           // [A][Form::Meander]
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0, 0.1f, 100e6f),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0, 3.33333e-8f, 10e6f),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            },
            {                                                                           // [A][Form::Impulse]
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0, 0.1f, 100e6f),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0, 3.33333e-8f, 10e6f),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            }

        },
        {
            {                                                                           // [B][Form::Sine]
                InputWindow::Struct(' ', "40000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0, 0.1f, 100e6f),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0, 3.3333333333e-8f),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One,  Wave::Parameter::Amplitude, 1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One,  Wave::Parameter::Offset,    1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Phase,     1, 0),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::One,  Wave::Parameter::Modulation, 1, 0, 0.0f, 10.0f)
            },
            {                                                                           // [B][Form::RampPlus]
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0, 0.1f, 100e6f),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0, 3.33333e-8f, 10e6f),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            },
            {                                                                           // [B][Form::RampMinus]
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0, 0.1f, 100e6f),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0, 3.33333e-8f, 10e6f),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            },
            {                                                                           // [B][Form::Meander]
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0, 0.1f, 100e6f),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0, 3.33333e-8f, 10e6f),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            },
            {                                                                           // [A][Form::Impulse]
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, 1, 0, 0.1f, 100e6f),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    1, 0, 3.33333e-8f, 10e6f),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, 1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    1, 0, 0.0f, 10.0f),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            }
        }
};


InputWindow::Struct param[3][2] =
{
    {InputWindow::Struct(' '), InputWindow::Struct(' ')},
    {InputWindow::Struct(' '), InputWindow::Struct(' ')},
    {InputWindow::Struct(' '), InputWindow::Struct(' ')}
};
