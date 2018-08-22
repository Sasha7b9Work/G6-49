#include "globals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputWindowStruct sig_structParameter[Chan::Number][WaveForm::Number][WaveParameter::Number] =
{
            {
            {                                                                           // [A][Form_Sine]
/* Frequency */ {"20000", "", 0, 0, NUM_DIGITS - 1, Order::Kilo,  WaveParameter::Frequency, SignValue::None, 1, 0},
/* Period    */ {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Milli, WaveParameter::Period,    SignValue::None, 1, 0},
/* Amplitude */ {"50000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::Amplitude, SignValue::None, 1, 0},
/* Offset    */ {"00000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::Offset,    SignValue::Plus, 1, 0},
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
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Kilo,  WaveParameter::Frequency, SignValue::None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Milli, WaveParameter::Period,    SignValue::None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::Amplitude, SignValue::None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::Offset,    SignValue::Plus, 1, 0},
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
                {"20000", "", 0, 0, NUM_DIGITS - 1, Order::Kilo,  WaveParameter::Frequency, SignValue::None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Milli, WaveParameter::Period,    SignValue::None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::Amplitude, SignValue::None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::Offset,    SignValue::Plus, 1, 0},
                {"10000", "", 2, 0, NUM_DIGITS - 1, Order::Micro, WaveParameter::Duration,  SignValue::None, 1, 0},
                {"20000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::DutyRatio, SignValue::None, 1, 0},
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
                {"20001", "", 0, 0, NUM_DIGITS - 1, Order::Kilo,  WaveParameter::Frequency, SignValue::None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Milli, WaveParameter::Period,    SignValue::None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::Amplitude, SignValue::None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::Offset,    SignValue::Plus, 1, 0},
                {},
                {},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::Phase,     SignValue::Plus, 1, 0},
                {}
            },
            {                                                                           // [B][Form_Saw]
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Kilo,  WaveParameter::Frequency, SignValue::None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Milli, WaveParameter::Period,    SignValue::None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::Amplitude, SignValue::None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::Offset,    SignValue::Plus, 1, 0},
                {},
                {},
                {},
                {}
            },
            {                                                                           // [B][Form_Impulse]
                {"20000", "", 0, 0, NUM_DIGITS - 1, Order::Kilo,  WaveParameter::Frequency, SignValue::None, 1, 0},
                {"10000", "", 0, 0, NUM_DIGITS - 1, Order::Milli, WaveParameter::Period,    SignValue::None, 1, 0},
                {"50000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::Amplitude, SignValue::None, 1, 0},
                {"00000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::Offset,    SignValue::Plus, 1, 0},
                {"10000", "", 2, 0, NUM_DIGITS - 1, Order::Micro, WaveParameter::Duration,  SignValue::None, 1, 0},
                {"20000", "", 0, 0, NUM_DIGITS - 1, Order::One,   WaveParameter::DutyRatio, SignValue::None, 1, 0},
                {},
                {"50000", "", 1, 0, NUM_DIGITS - 1, Order::Micro, WaveParameter::Delay,     SignValue::None, 1, 0}
            }
        }
};
