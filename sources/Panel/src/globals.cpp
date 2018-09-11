#include "globals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputWindow::Struct sig_structParameter[Chan::Number][Form::Number][Parameter::Number] =
{
        {
            {                                                                           // [A][Form::Sine]
/* Frequency  */ InputWindow::Struct(' ', "20000", 0, NUM_DIGITS - 1, Order::Kilo),
/* Period     */ InputWindow::Struct(' ', "05000", 0, NUM_DIGITS - 1, Order::Milli),
/* Amplitude  */ InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One),
/* Offset     */ InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One),
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
/* Modulation */ InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::One)
            },
            {                                                                           // [A][Form::RampPlus]
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One),
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
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One),
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
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One),
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
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One),
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
                InputWindow::Struct(' ', "40000", 0, NUM_DIGITS - 1, Order::Kilo),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::One),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::One)
            },
            {                                                                           // [B][Form::RampPlus]
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One),
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
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One),
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
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One),
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
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Kilo),
                InputWindow::Struct(' ', "10000", 0, NUM_DIGITS - 1, Order::Milli),
                InputWindow::Struct(' ', "10000", 1, NUM_DIGITS - 1, Order::One),
                InputWindow::Struct(' ', "50000", 0, NUM_DIGITS - 1, Order::One),
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
