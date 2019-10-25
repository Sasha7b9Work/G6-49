#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Signals.h"
#include "Display/Painter.h"
#include "Menu/Pages/Addition/PageLoadForm.h"
#include "Menu/Pages/Include/PageSignals.h"
#endif



static ParameterChoice param_SineModulationA_Manipulation        (ParameterChoice::ManipulationEnabled, DISABLED_RU, ENABLED_RU);
static ParameterValue  param_SineModulationA_ManipulationDuration(ParameterValue::ManipulationDuration, 0.0F,  10e3F,  "5000", 0, Order::Milli);
static ParameterValue  param_SineModulationA_ManipulationPeriod  (ParameterValue::ManipulationPeriod,   0.0F,  10e3F,  "2500", 1, Order::Milli);
static ParameterValue  param_SineModulationA_Exit                (ParameterValue::Exit,                 -1.0F, 1.0F,   "",     0, Order::One);


static ParameterBase *params_SineModulationA[] =
{
    &param_SineModulationA_Manipulation,
    &param_SineModulationA_ManipulationDuration,
    &param_SineModulationA_ManipulationPeriod,
    &param_SineModulationA_Exit,
    0
};

static ParameterValue   param_SineA_Frequency   (ParameterValue::Frequency,      0.1F,  100e6F, "10000", 0, Order::Kilo);
static ParameterValue   param_SineA_Amplitude   (ParameterValue::Amplitude,      0.0F,  10.0F,  "50000", 0, Order::One);
static ParameterValue   param_SineA_Offset      (ParameterValue::Offset,         0.0F,  10.0F,  "50000", 0, Order::One);
static ParameterComplex param_SineA_Manipulation(ParameterComplex::Manipulation, params_SineModulationA);

static ParameterBase *params_SineA[] =
{
    &param_SineA_Frequency,
    &param_SineA_Amplitude,
    &param_SineA_Offset,
    &param_SineA_Manipulation,
    0
};

static Form formSineA(Form::Sine, params_SineA, &waves[Chan::A]);


static ParameterValue  param_RampPlusA_Frequency(ParameterValue::Frequency, 0.1F, 100e6F, "10000",  0, Order::Kilo);
static ParameterValue  param_RampPlusA_Amplitude(ParameterValue::Amplitude, 0.0F, 10.0F,  "10000",  1, Order::One);
static ParameterValue  param_RampPlusA_Offset   (ParameterValue::Offset,    0.0F, 10.0F,  "50000",  0, Order::One);
static ParameterChoice param_RampPlusA_ModeStart(ParameterChoice::ModeStart, " Авто", " Однокр", " Комп А", " Форм B");

static ParameterBase *params_RampPlusA[] =
{
    &param_RampPlusA_Frequency,
    &param_RampPlusA_Amplitude,
    &param_RampPlusA_Offset,
    &param_RampPlusA_ModeStart,
    0
};

static Form formRampPlusA(Form::RampPlus, params_RampPlusA, &waves[Chan::A]);


static ParameterValue  param_RampMinusA_Frequency(ParameterValue::Frequency, 0.1F, 100e6F, "10000", 0, Order::Kilo);
static ParameterValue  param_RampMinusA_Amplitude(ParameterValue::Amplitude, 0.0F, 10.0F,  "10000", 1, Order::One);
static ParameterValue  param_RampMinusA_Offset   (ParameterValue::Offset,    0.0F, 10.0F,  "50000", 0, Order::One);
static ParameterChoice param_RampMinusA_ModeStart(ParameterChoice::ModeStart, " Авто", " Однокр", " Комп А", " Форм B");

static ParameterBase *params_RampMinusA[] =
{
    &param_RampMinusA_Frequency,
    &param_RampMinusA_Amplitude,
    &param_RampMinusA_Offset,
    &param_RampMinusA_ModeStart,
    0
};

static Form formRampMinusA(Form::RampMinus, params_RampMinusA, &waves[Chan::A]);


static ParameterValue  param_TriangleA_Frequency(ParameterValue::Frequency,  0.1F, 100e6F, "10000", 0, Order::Kilo);
static ParameterValue  param_TriangleA_Amplitude(ParameterValue::Amplitude,  0.0F, 10.0F, "10000",  1, Order::One);
static ParameterValue  param_TriangleA_Offset   (ParameterValue::Offset,     0.0F, 10.0F, "50000",  0, Order::One);
static ParameterChoice param_TriangleA_ModeStart(ParameterChoice::ModeStart, " Авто", " Однокр", " Комп А", " Форм B");

static ParameterBase *params_TriangleA[] =
{
    &param_TriangleA_Frequency,
    &param_TriangleA_Amplitude,
    &param_TriangleA_Offset,
    &param_TriangleA_ModeStart,
    0
};

static Form formTriangleA(Form::Triangle, params_TriangleA, &waves[Chan::A]);


static ParameterValue  param_DDSA_Frequency(ParameterValue::Frequency,  0.1F, 100e6F, "10000", 0, Order::Kilo);
static ParameterValue  param_DDSA_Amplitude(ParameterValue::Amplitude,  0.0F, 10.0F, "10000", 1, Order::One);
static ParameterValue  param_DDSA_Offset   (ParameterValue::Offset,     0.0F, 10.0F, "50000", 0, Order::One);
static ParameterChoice param_DDSA_ModeStart(ParameterChoice::ModeStart, " Авто", " Однокр", " Комп А", " Форм B");
static ParameterPage   param_DDSA_Choice   (ParameterPage::ChoiceForm, reinterpret_cast<PageBase *>(PageLoadForm::pointer));

static ParameterBase *params_DDSA[] =
{
    &param_DDSA_Frequency,
    &param_DDSA_Amplitude,
    &param_DDSA_Offset,
    &param_DDSA_ModeStart,
    &param_DDSA_Choice,
    0
};

static Form formDDSA(Form::DDS, params_DDSA, &waves[Chan::A]);



static ParameterValue param_MeanderA_Frequency(ParameterValue::Frequency, 0.1F, 100e6F, "10000", 0, Order::Kilo);
static ParameterValue param_MeanderA_Amplitude(ParameterValue::Amplitude, 0.0F, 10.0F,  "10000", 1, Order::One);
static ParameterValue param_MeanderA_Offset   (ParameterValue::Offset,    0.0F, 10.0F,  "50000", 0, Order::One);

static ParameterBase *params_MeanderA[] =
{
    &param_MeanderA_Frequency,
    &param_MeanderA_Amplitude,
    &param_MeanderA_Offset,
    0
};

static Form formMeanderA(Form::Meander, params_MeanderA, &waves[Chan::A]);



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

static ParameterValue  param_ImpulseA_Period   (ParameterValue::Period,    3.3e-8F, 10e6F,  "10000", 0, Order::Milli);
static ParameterValue  param_ImpulseA_Duration (ParameterValue::Duration,  0.0F,    100e6F, "01000", 0, Order::Milli);
static ParameterValue  param_ImpulseA_Amplitude(ParameterValue::Amplitude, 0.0F,    10.0F,  "10000", 1, Order::One);
static ParameterValue  param_ImpulseA_Offset   (ParameterValue::Offset,    0.0F,    10.0F,  "50000", 0, Order::One);
static ParameterChoice param_ImpulseA_Polarity (ParameterChoice::Polarity, "", "", DrawPlus, DrawMinus);
static ParameterChoice param_ImpulseA_ModeStart(ParameterChoice::ModeStart, " Авто", " Однокр", " Комп А", " Форм B");

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

static Form formImpulseA(Form::Impulse, params_ImpulseA, &waves[Chan::A]);



static ParameterValue  param_PacketA_Period      (ParameterValue::Period,       0.0F, 100e6F, "02000", 0, Order::Milli);
static ParameterValue  param_PacketA_Duration    (ParameterValue::Duration,     0.0F, 100e6F, "01000", 0, Order::Milli);
static ParameterValue  param_PacketA_PacketNumber(ParameterValue::PacketNumber, 0.0F, 100e6F, "30000", 0, Order::One);
static ParameterValue  param_PacketA_PacketPeriod(ParameterValue::PacketPeriod, 0.0F, 100e6F, "10000", 0, Order::Milli);
static ParameterValue  param_PacketA_Amplitude   (ParameterValue::Amplitude,    0.0F, 10.0F,  "10000", 1, Order::One);
static ParameterValue  param_PacketA_Offset      (ParameterValue::Offset,       0.0F, 10.0F,  "50000", 0, Order::One);
static ParameterChoice param_PacketA_Polarity    (ParameterChoice::Polarity,    "", "", DrawPlus, DrawMinus);
static ParameterChoice param_PacketA_ModeStart   (ParameterChoice::ModeStart,   " Авто", " Однокр", " Комп А", " Форм B");

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

static Form formPacketImpulseA(Form::PacketImpuls, params_PacketA, &waves[Chan::A]);



extern Wave waves[Chan::Count];

static Form *formsA[] =
{
    &formSineA,
    &formRampPlusA,
    &formRampMinusA,
    &formTriangleA,
    &formMeanderA,
    &formImpulseA,
    &formPacketImpulseA,
    &formDDSA,
    0
};



static ParameterChoice param_SineModulationB_Manipulation        (ParameterChoice::ManipulationEnabled, DISABLED_RU, ENABLED_RU);
static ParameterValue  param_SineModulationB_ManipulationDuration(ParameterValue::ManipulationDuration, 0.0F,  10e3F,  "5000", 0, Order::Milli);
static ParameterValue  param_SineModulationB_ManipulationPeriod  (ParameterValue::ManipulationPeriod,   0.0F,  10e3F,  "2500", 1, Order::Milli);
static ParameterValue  param_SineModulationB_Exit                (ParameterValue::Exit,                 -1.0F, 1.0F,   "",     0, Order::One);

static ParameterBase *params_SineModulationB[] =
{
    &param_SineModulationB_Manipulation,
    &param_SineModulationB_ManipulationDuration,
    &param_SineModulationB_ManipulationPeriod,
    &param_SineModulationB_Exit,
    0
};

static ParameterValue   param_SineB_Frequency   (ParameterValue::Frequency,      0.1F,  100e6F, "10000", 0, Order::Kilo);
static ParameterValue   param_SineB_Amplitude   (ParameterValue::Amplitude,      0.0F,  10.0F,  "10000", 1, Order::One);
static ParameterValue   param_SineB_Offset      (ParameterValue::Offset,         0.0F,  10.0F,  "50000", 0, Order::One);
static ParameterValue   param_SineB_Phase       (ParameterValue::Phase,          0.0F,  360.0F, "00000", 0, Order::One);
static ParameterComplex param_SineB_Manipulation(ParameterComplex::Manipulation, params_SineModulationB);

static ParameterBase *params_SineB[] =
{
    &param_SineB_Frequency,
    &param_SineB_Amplitude,
    &param_SineB_Offset,
    &param_SineB_Phase,
    &param_SineB_Manipulation,
    0
};

static Form formSineB(Form::Sine, params_SineB, &waves[Chan::B]);


static ParameterValue  param_RampPlusB_Frequency(ParameterValue::Frequency, 0.1F, 100e6F, "10000", 0, Order::Kilo);
static ParameterValue  param_RampPlusB_Amplitude(ParameterValue::Amplitude, 0.0F, 10.0F,  "50000", 0, Order::One);
static ParameterValue  param_RampPlusB_Offset   (ParameterValue::Offset,    0.0F, 10.0F,  "50000", 0, Order::One);
static ParameterChoice param_RampPlusB_ModeStart(ParameterChoice::ModeStart, " Авто", " Однокр", " Комп А", " Форм B");

static ParameterBase *params_RampPlusB[] =
{
    &param_RampPlusB_Frequency,
    &param_RampPlusB_Amplitude,
    &param_RampPlusB_Offset,
    &param_RampPlusB_ModeStart,
    0
};

static Form formRampPlusB(Form::RampPlus, params_RampPlusB, &waves[Chan::B]);


static ParameterValue  param_RampMinusB_Frequency(ParameterValue::Frequency,  0.1F, 100e6F, "10000", 0, Order::Kilo);
static ParameterValue  param_RampMinusB_Amplitude(ParameterValue::Amplitude,  0.0F, 10.0F,  "50000", 0, Order::One);
static ParameterValue  param_RampMinusB_Offset   (ParameterValue::Offset,     0.0F, 10.0F,  "50000", 0, Order::One);
static ParameterChoice param_RampMinusB_ModeStart(ParameterChoice::ModeStart, " Авто", " Однокр", " Комп А", " Форм B");

static ParameterBase *params_RampMinusB[] =
{
    &param_RampMinusB_Frequency,
    &param_RampMinusB_Amplitude,
    &param_RampMinusB_Offset,
    &param_RampMinusB_ModeStart,
    0
};

static Form formRampMinusB(Form::RampMinus, params_RampMinusB, &waves[Chan::B]);


static ParameterValue  param_TriangleB_Frequency(ParameterValue::Frequency,  0.1F, 100e6F, "10000", 0, Order::Kilo);
static ParameterValue  param_TriangleB_Amplitude(ParameterValue::Amplitude,  0.0F, 10.0F,  "50000", 0, Order::One);
static ParameterValue  param_TriangleB_Offset   (ParameterValue::Offset,     0.0F, 10.0F,  "50000", 0, Order::One);
static ParameterChoice param_TriangleB_ModeStart(ParameterChoice::ModeStart, " Авто", " Однокр", " Комп А", " Форм B");

static ParameterBase *params_TriangleB[] =
{
    &param_TriangleB_Frequency,
    &param_TriangleB_Amplitude,
    &param_TriangleB_Offset,
    &param_TriangleB_ModeStart,
    0
};

static Form formTriangleB(Form::Triangle, params_TriangleB, &waves[Chan::B]);


static ParameterValue  param_DDSB_Frequency(ParameterValue::Frequency,  0.1F, 100e6F, "10000", 0, Order::Kilo);
static ParameterValue  param_DDSB_Amplitude(ParameterValue::Amplitude,  0.0F, 10.0F,  "50000", 0, Order::One);
static ParameterValue  param_DDSB_Offset   (ParameterValue::Offset,     0.0F, 10.0F,  "50000", 0, Order::One);
static ParameterChoice param_DDSB_ModeStart(ParameterChoice::ModeStart, " Авто", " Однокр", " Комп А", " Форм B");
static ParameterPage   param_DDSB_Choice   (ParameterPage::ChoiceForm, reinterpret_cast<PageBase *>(PageLoadForm::pointer));

static ParameterBase *params_DDSB[] =
{
    &param_DDSB_Frequency,
    &param_DDSB_Amplitude,
    &param_DDSB_Offset,
    &param_DDSB_ModeStart,
    &param_DDSB_Choice,
    0
};

static Form formDDSB(Form::DDS, params_DDSB, &waves[Chan::B]);


static ParameterValue param_MeanderB_Frequency(ParameterValue::Frequency, 0.1F, 100e6F, "10000", 0, Order::Kilo);
static ParameterValue param_MeanderB_Amplitude(ParameterValue::Amplitude, 0.0F, 10.0F,  "10000", 1, Order::One);
static ParameterValue param_MeanderB_Offset   (ParameterValue::Offset,    0.0F, 10.0F,  "50000", 0, Order::One);

static ParameterBase *params_MeanderB[] =
{
    &param_MeanderB_Frequency,
    &param_MeanderB_Amplitude,
    &param_MeanderB_Offset,
    0
};

static Form formMeanderB(Form::Meander, params_MeanderB, &waves[Chan::B]);



static ParameterValue  param_ImpulseB_Period   (ParameterValue::Period,    3.3e-8F, 10e6F,  "10000", 0, Order::Milli);
static ParameterValue  param_ImpulseB_Duration (ParameterValue::Duration,  0.0F,    100e6F, "01000", 0, Order::Milli);
static ParameterValue  param_ImpulseB_Amplitude(ParameterValue::Amplitude, 0.0F,    10.0F,  "10000", 1, Order::One);
static ParameterValue  param_ImpulseB_Offset   (ParameterValue::Offset,    0.0F,    10.0F,  "50000", 0, Order::One);
static ParameterChoice param_ImpulseB_Polarity (ParameterChoice::Polarity,  "", "", DrawPlus, DrawMinus);
static ParameterChoice param_ImpulseB_ModeStart(ParameterChoice::ModeStart, " Авто", " Однокр", " Комп А", " Форм B");

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

static Form formImpulseB(Form::Impulse, params_ImpulseB, &waves[Chan::B]);



extern Wave waves[Chan::Count];

static Form *formsB[] =
{
    &formSineB,
    &formRampPlusB,
    &formRampMinusB,
    &formTriangleB,
    &formMeanderB,
    &formImpulseB,
    &formDDSB,
    0
};



Wave waves[Chan::Count] =
{
    Wave(Chan::A, formsA),
    Wave(Chan::B, formsB)
};
