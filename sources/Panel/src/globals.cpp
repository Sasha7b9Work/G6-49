#include "globals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputWindow::Struct sig_structParameter[Chan::Number][Form::Number][Parameter::Number] =
{
        {
            {                                                                           // [A][Form::Sine]
/* Frequency  */ InputWindow::Struct(' '),
/* Period     */ InputWindow::Struct(' '),
/* Amplitude  */ InputWindow::Struct(' '),
/* Offset     */ InputWindow::Struct(' '),
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
/* Modulation */ InputWindow::Struct(' ')
            },
            {                                                                           // [A][Form::RampPlus]
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
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            },
            {                                                                           // [A][Form::RampMinus]
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
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            },
            {                                                                           // [A][Form::Meander]
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
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            },
            {                                                                           // [A][Form::Impulse]
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
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            }

        },
        {
            {                                                                           // [B][Form::Sine]
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
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            },
            {                                                                           // [B][Form::RampPlus]
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
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            },
            {                                                                           // [B][Form::RampMinus]
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
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            },
            {                                                                           // [B][Form::Meander]
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
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' '),
                InputWindow::Struct(' ')
            },
            {                                                                           // [B][Form::Impulse]
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
