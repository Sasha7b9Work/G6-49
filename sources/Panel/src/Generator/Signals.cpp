#include "Signals.h"
#include "Display/Painter.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawRawImpulse(int x, int y)
{
    Painter::DrawHLine(y + 8, x, x + 8);
    Painter::DrawLine(x + 12, y, x + 8, y + 8);
    Painter::DrawHLine(y, x + 12, x + 20);
    Painter::DrawLine(x + 20, y, x + 24, y + 8);
    Painter::DrawHLine(y + 8, x + 24, x + 32);
}

static void DrawRectangleImpulse(int x, int y)
{
    Painter::DrawHLine(y + 8, x, x + 8);
    Painter::DrawVLine(x + 8, y, y + 8);
    Painter::DrawHLine(y, x + 8, x + 24);
    Painter::DrawVLine(x + 24, y, y + 8);
    Painter::DrawHLine(y + 8, x + 24, x + 32);
}

static ParameterValue param_SineModulationA_Manipulation        (ParameterValue::Manipulation,         -1.0f, 1.0f,   "",     0, Order::One);
static ParameterChoice param_SineModulationA_ManipulationMode   (ParameterChoice::ModeManipulation, "", "", "", "", 
                                                                                                    DrawRawImpulse, DrawRectangleImpulse);
static ParameterValue param_SineModulationA_ManipulationDuration(ParameterValue::ManipulationDuration, 0.0f,  10e3f,  "5000", 0, Order::Milli);
static ParameterValue param_SineModulationA_ManipulationPeriod  (ParameterValue::ManipulationPeriod,   0.0f,  10e3f,  "2500", 1, Order::Milli);
static ParameterValue param_SineModulationA_Exit                (ParameterValue::Exit,                 -1.0f, 1.0f,   "",     0, Order::One);


static ParameterBase *params_SineModulationA[] =
{
    &param_SineModulationA_Manipulation,
    &param_SineModulationA_ManipulationMode,
    &param_SineModulationA_ManipulationDuration,
    &param_SineModulationA_ManipulationPeriod,
    &param_SineModulationA_Exit,
    0
};

static ParameterValue param_SineA_Frequency   (ParameterValue::Frequency,    0.1f,  100e6f, "10000", 0, Order::Kilo);
static ParameterValue param_SineA_Amplitude   (ParameterValue::Amplitude,    0.0f,  10.0f,  "50000", 0, Order::One);
static ParameterValue param_SineA_Offset      (ParameterValue::Offset,       0.0f,  10.0f,  "50000", 0, Order::One);
static ParameterValue param_SineA_Manipulation(ParameterValue::Manipulation, -1.0f, -1.0f,  "",      0, Order::One, params_SineModulationA);

static ParameterBase *params_SineA[] =
{
    &param_SineA_Frequency,
    &param_SineA_Amplitude,
    &param_SineA_Offset,
    &param_SineA_Manipulation,
    0
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static ParameterValue  param_RampPlusA_Frequency(ParameterValue::Frequency, 0.1f, 100e6f, "10000",  0, Order::Kilo);
static ParameterValue  param_RampPlusA_Amplitude(ParameterValue::Amplitude, 0.0f, 10.0f,  "10000",  1, Order::One);
static ParameterValue  param_RampPlusA_Offset   (ParameterValue::Offset,    0.0f, 10.0f,  "50000",  0, Order::One);
static ParameterChoice param_RampPlusA_ModeStart(ParameterChoice::ModeStart, " Авто",   " Auto",
                                                                             " Однокр", " Single",
                                                                             " Комп А", " Comp A",
                                                                             " Форм B", " Shaper B");

static ParameterBase *params_RampPlusA[] =
{
    &param_RampPlusA_Frequency,
    &param_RampPlusA_Amplitude,
    &param_RampPlusA_Offset,
    &param_RampPlusA_ModeStart,
    0
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static ParameterValue  param_RampMinusA_Frequency(ParameterValue::Frequency, 0.1f, 100e6f, "10000", 0, Order::Kilo);
static ParameterValue  param_RampMinusA_Amplitude(ParameterValue::Amplitude, 0.0f, 10.0f,  "10000", 1, Order::One);
static ParameterValue  param_RampMinusA_Offset   (ParameterValue::Offset,    0.0f, 10.0f,  "50000", 0, Order::One);
static ParameterChoice param_RampMinusA_ModeStart(ParameterChoice::ModeStart, " Авто",   " Auto",
                                                                              " Однокр", " Single",
                                                                              " Комп А", " Comp A",
                                                                              " Форм B", " Shaper B");

static ParameterBase *params_RampMinusA[] =
{
    &param_RampMinusA_Frequency,
    &param_RampMinusA_Amplitude,
    &param_RampMinusA_Offset,
    &param_RampMinusA_ModeStart,
    0
};



//----------------------------------------------------------------------------------------------------------------------------------------------------
static ParameterValue param_MeanderA_Frequency(ParameterValue::Frequency, 0.1f, 100e6f, "10000", 0, Order::Kilo);
static ParameterValue param_MeanderA_Amplitude(ParameterValue::Amplitude, 0.0f, 10.0f,  "10000", 1, Order::One);
static ParameterValue param_MeanderA_Offset   (ParameterValue::Offset,    0.0f, 10.0f,  "50000", 0, Order::One);

static ParameterBase *params_MeanderA[] =
{
    &param_MeanderA_Frequency,
    &param_MeanderA_Amplitude,
    &param_MeanderA_Offset,
    0
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawPlus(int x, int y)
{
    Painter::DrawHLine(y + 8, x, x + 8);
    Painter::DrawVLine(x + 8, y, y + 8);
    Painter::DrawHLine(y, x + 8, x + 16);
    Painter::DrawVLine(x + 16, y, y + 8);
    Painter::DrawHLine(y + 8, x + 16, x + 24);
}

static void DrawMinus(int x, int y)
{
    Painter::DrawHLine(y, x, x + 8);
    Painter::DrawVLine(x + 8, y, y + 8);
    Painter::DrawHLine(y + 8, x + 8, x + 16);
    Painter::DrawVLine(x + 16, y, y + 8);
    Painter::DrawHLine(y, x + 16, x + 24);
}

static ParameterValue  param_ImpulseA_Period   (ParameterValue::Period,    3.3e-8f, 10e6f,  "10000", 0, Order::Milli);
static ParameterValue  param_ImpulseA_Duration (ParameterValue::Duration,  0.0f,    100e6f, "01000", 0, Order::Milli);
static ParameterValue  param_ImpulseA_Amplitude(ParameterValue::Amplitude, 0.0f,    10.0f,  "10000", 1, Order::One);
static ParameterValue  param_ImpulseA_Offset   (ParameterValue::Offset,    0.0f,    10.0f,  "50000", 0, Order::One);
static ParameterChoice param_ImpulseA_Polarity(ParameterChoice::Polarity,  "", "", "", "", DrawPlus, DrawMinus);
static ParameterChoice param_ImpulseA_ModeStart(ParameterChoice::ModeStart, " Авто",   " Auto",
                                                                            " Однокр", " Single",
                                                                            " Комп А", " Comp A",
                                                                            " Форм B", " Shaper B");

static ParameterBase *params_ImpulseA[] =
{
    &param_ImpulseA_Period,
    &param_ImpulseA_Duration,
    &param_ImpulseA_Amplitude,
    &param_ImpulseA_Offset,
    &param_ImpulseA_Polarity,
    &param_ImpulseA_ModeStart,
    0
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static ParameterValue  param_PacketA_Period      (ParameterValue::Period,       0.0f, 100e6f, "02000", 0, Order::Milli);
static ParameterValue  param_PacketA_Duration    (ParameterValue::Duration,     0.0f, 100e6f, "01000", 0, Order::Milli);
static ParameterValue  param_PacketA_PacketNumber(ParameterValue::PacketNumber, 0.0f, 100e6f, "30000", 0, Order::One);
static ParameterValue  param_PacketA_PacketPeriod(ParameterValue::PacketPeriod, 0.0f, 100e6f, "10000", 0, Order::Milli);
static ParameterValue  param_PacketA_Amplitude   (ParameterValue::Amplitude,    0.0f, 10.0f,  "10000", 1, Order::One);
static ParameterValue  param_PacketA_Offset      (ParameterValue::Offset,       0.0f, 10.0f,  "50000", 0, Order::One);
static ParameterChoice param_PacketA_Polarity    (ParameterChoice::Polarity,    "", "", "", "", DrawPlus, DrawMinus);
static ParameterChoice param_PacketA_ModeStart   (ParameterChoice::ModeStart,   " Авто",   " Auto",
                                                                                " Однокр", " Single",
                                                                                " Комп А", " Comp A",
                                                                                " Форм B", " Shaper B");

static ParameterBase *params_PacketA[] =
{
    &param_PacketA_Period,
    &param_PacketA_Duration,
    &param_PacketA_PacketNumber,
    &param_PacketA_PacketPeriod,
    &param_PacketA_Amplitude,
    &param_PacketA_Offset,
    &param_PacketA_Polarity,
    &param_PacketA_ModeStart,
    0
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
extern Wave wave[Chan::Number];

static Form formsA[] =
{
    Form(Form::Sine,            params_SineA,      &wave[Chan::A]),
    Form(Form::RampPlus,        params_RampPlusA,  &wave[Chan::A]),
    Form(Form::RampMinus,       params_RampMinusA, &wave[Chan::A]),
    Form(Form::Meander,         params_MeanderA,   &wave[Chan::A]),
    Form(Form::Impulse,         params_ImpulseA,   &wave[Chan::A]),
    Form(Form::PacketImpuls,    params_PacketA,    &wave[Chan::A])
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static ParameterValue param_SineModulationB_Manipulation        (ParameterValue::Manipulation,         -1.0f, 1.0f,   "",     0, Order::One);
static ParameterValue param_SineModulationB_ManipulationDuration(ParameterValue::ManipulationDuration, 0.0f,  10e3f,  "5000", 0, Order::Milli);
static ParameterValue param_SineModulationB_ManipulationPeriod  (ParameterValue::ManipulationPeriod,   0.0f,  10e3f,  "2500", 1, Order::Milli);
static ParameterValue param_SineModulationB_Exit                (ParameterValue::Exit,                 -1.0f, 1.0f,   "",     0, Order::One);

static ParameterBase *params_SineModulationB[] =
{
    &param_SineModulationB_Manipulation,
    &param_SineModulationB_ManipulationDuration,
    &param_SineModulationB_ManipulationPeriod,
    &param_SineModulationB_Exit,
    0
};

static ParameterValue param_SineB_Frequency   (ParameterValue::Frequency,    0.1f,  100e6f, "10000", 0, Order::Kilo);
static ParameterValue param_SineB_Amplitude   (ParameterValue::Amplitude,    0.0f,  10.0f,  "10000", 1, Order::One);
static ParameterValue param_SineB_Offset      (ParameterValue::Offset,       0.0f,  10.0f,  "50000", 0, Order::One);
static ParameterValue param_SineB_Phase       (ParameterValue::Phase,        0.0f,  360.0f, "00000", 0, Order::One);
static ParameterValue param_SineB_Manipulation(ParameterValue::Manipulation, -1.0f, -1.0f,  "10000", 0, Order::One, params_SineModulationB);

static ParameterBase *params_SineB[] =
{
    &param_SineB_Frequency,
    &param_SineB_Amplitude,
    &param_SineB_Offset,
    &param_SineB_Phase,
    &param_SineB_Manipulation,
    0
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static ParameterValue  param_RampPlusB_Frequency(ParameterValue::Frequency, 0.1f, 100e6f, "10000", 0, Order::Kilo);
static ParameterValue  param_RampPlusB_Amplitude(ParameterValue::Amplitude, 0.0f, 10.0f,  "50000", 0, Order::One);
static ParameterValue  param_RampPlusB_Offset   (ParameterValue::Offset,    0.0f, 10.0f,  "50000", 0, Order::One);
static ParameterChoice param_RampPlusB_ModeStart(ParameterChoice::ModeStart, " Авто",   " Auto",
                                                                             " Однокр", " Single",
                                                                             " Комп А", " Comp A",
                                                                             " Форм B", " Shaper B");

static ParameterBase *params_RampPlusB[] =
{
    &param_RampPlusB_Frequency,
    &param_RampPlusB_Amplitude,
    &param_RampPlusB_Offset,
    &param_RampPlusB_ModeStart,
    0
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static ParameterValue  param_RampMinusB_Frequency(ParameterValue::Frequency, 0.1f, 100e6f, "10000", 0, Order::Kilo);
static ParameterValue  param_RampMinusB_Amplitude(ParameterValue::Amplitude, 0.0f, 10.0f,  "50000", 0, Order::One);
static ParameterValue  param_RampMinusB_Offset   (ParameterValue::Offset,    0.0f, 10.0f,  "50000", 0, Order::One);
static ParameterChoice param_RampMinusB_ModeStart(ParameterChoice::ModeStart, " Авто",   " Auto",
                                                                              " Однокр", " Single",
                                                                              " Комп А", " Comp A",
                                                                              " Форм B", " Shaper B");

static ParameterBase *params_RampMinusB[] =
{
    &param_RampMinusB_Frequency,
    &param_RampMinusB_Amplitude,
    &param_RampMinusB_Offset,
    &param_RampMinusB_ModeStart,
    0
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static ParameterValue param_MeanderB_Frequency(ParameterValue::Frequency, 0.1f, 100e6f, "10000", 0, Order::Kilo);
static ParameterValue param_MeanderB_Amplitude(ParameterValue::Amplitude, 0.0f, 10.0f,  "10000", 1, Order::One);
static ParameterValue param_MeanderB_Offset   (ParameterValue::Offset,    0.0f, 10.0f,  "50000", 0, Order::One);

static ParameterBase *params_MeanderB[] =
{
    &param_MeanderB_Frequency,
    &param_MeanderB_Amplitude,
    &param_MeanderB_Offset,
    0
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static ParameterValue  param_ImpulseB_Period   (ParameterValue::Period,    3.3e-8f, 10e6f,  "10000", 0, Order::Milli);
static ParameterValue  param_ImpulseB_Duration (ParameterValue::Duration,  0.0f,    100e6f, "01000", 0, Order::Milli);
static ParameterValue  param_ImpulseB_Amplitude(ParameterValue::Amplitude, 0.0f,    10.0f,  "10000", 1, Order::One);
static ParameterValue  param_ImpulseB_Offset   (ParameterValue::Offset,    0.0f,    10.0f,  "50000", 0, Order::One);
static ParameterChoice param_ImpulseB_Polarity (ParameterChoice::Polarity,  "", "", "", "", DrawPlus, DrawMinus);
static ParameterChoice param_ImpulseB_ModeStart(ParameterChoice::ModeStart, " Авто",   " Auto",
                                                                            " Однокр", " Single",
                                                                            " Комп А", " Comp A",
                                                                            " Форм B", " Shaper B");

static ParameterBase *params_ImpulseB[] =
{
    &param_ImpulseB_Period,
    &param_ImpulseB_Duration,
    &param_ImpulseB_Amplitude,
    &param_ImpulseB_Offset,
    &param_ImpulseB_Polarity,
    &param_ImpulseB_ModeStart,
    0
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
extern Wave wave[Chan::Number];

static Form formsB[] =
{
    Form(Form::Sine,         params_SineB,      &wave[Chan::B]),
    Form(Form::RampPlus,     params_RampPlusB,  &wave[Chan::B]),
    Form(Form::RampMinus,    params_RampMinusB, &wave[Chan::B]),
    Form(Form::Meander,      params_MeanderB,   &wave[Chan::B]),
    Form(Form::Impulse,      params_ImpulseB,   &wave[Chan::B])
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Wave wave[Chan::Number] =
{
    Wave(Chan::A, formsA, 6),
    Wave(Chan::B, formsB, 5)
};
