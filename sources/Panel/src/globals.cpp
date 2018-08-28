#include "globals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputWindow::Struct sig_structParameter[Chan::Number][Wave::Form::Number][Wave::Parameter::Number] =
{
            {
            {                                                                           // [A][Form_Sine]
/* Frequency */ {"20000", "", 0, 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, SignValue::None, 1, 0},
/* Period    */ {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    SignValue::None, 1, 0},
/* Amplitude */ {"50000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, SignValue::None, 1, 0},
/* Offset    */ {"00000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    SignValue::Plus, 1, 0},
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
            {                                                                           // [A][Form_Saw]
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, SignValue::None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    SignValue::None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, SignValue::None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    SignValue::Plus, 1, 0},
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
            {                                                                           // [A][Form_Impulse]
                {"20000", "", 0, 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, SignValue::None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    SignValue::None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, SignValue::None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    SignValue::Plus, 1, 0},
                {"10000", "", 2, 0, NUM_DIGITS - 1, Order::Micro, Wave::Parameter::Duration,  SignValue::None, 1, 0},
                {"20000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::DutyRatio, SignValue::None, 1, 0},
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
            {                                                                           // [B][Form_Sine]
                {"20001", "", 0, 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, SignValue::None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    SignValue::None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, SignValue::None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    SignValue::Plus, 1, 0},
                {},
                {},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Phase,     SignValue::Plus, 1, 0},
                {}
            },
            {                                                                           // [B][Form_Saw]
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, SignValue::None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    SignValue::None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, SignValue::None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    SignValue::Plus, 1, 0},
                {},
                {},
                {},
                {}
            },
            {                                                                           // [B][Form_Impulse]
                {"20000", "", 0, 0, NUM_DIGITS - 1, Order::Kilo,  Wave::Parameter::Frequency, SignValue::None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Milli, Wave::Parameter::Period,    SignValue::None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Amplitude, SignValue::None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::Offset,    SignValue::Plus, 1, 0},
                {"10000", "", 2, 0, NUM_DIGITS - 1, Order::Micro, Wave::Parameter::Duration,  SignValue::None, 1, 0},
                {"20000", "", 0, 0, NUM_DIGITS - 1, Order::One,   Wave::Parameter::DutyRatio, SignValue::None, 1, 0},
                {},
                {"50000", "", 1, 0, NUM_DIGITS - 1, Order::Micro, Wave::Parameter::Delay,     SignValue::None, 1, 0}
            }
        }
};
