// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Generator/Signals.h"
#include "Generator/Parameters.h"
#include "Menu/Pages/Pages.h"


extern ParameterModeStart impulseA_ModeStart;

#define FREQUENCY_SINE_MIN  Value("300", Order::Micro)
#define FREQUENCY_SINE_MAX  Value("100", Order::Mega)

#define FREQUENCY_DDS_MIN   Value("100", Order::Micro)
#define FREQUENCY_DDS_MAX   Value("10",  Order::Mega)


static pchar namesManipulationEnabled[] =
{
    DISABLED_RU, DISABLED_EN,
    ENABLED_RU,  ENABLED_EN,
    nullptr
};


static void FuncCloseManipulation()
{
    CURRENT_FORM->CloseCompositeParameter();
}


static ParameterManipulationEnabled   sineManipulationA_Enabled (namesManipulationEnabled);
static ParameterManipulationDuration  sineManipulationA_Duration(Value("10", Order::Nano), Value("10", Order::One),
    Value("5", Order::Milli));
static ParameterManipulationPeriod    sineManipulationA_Period  (Value("20", Order::Nano), Value("10000", Order::One),
    Value("25", Order::Milli));
static ParameterButton                sineManipulationA_Exit    ("Закрыть ( ESC )", "Close ( ESC )", FuncCloseManipulation);


static Parameter *sineManipulationA[] =
{
    &sineManipulationA_Enabled,
    &sineManipulationA_Duration,
    &sineManipulationA_Period,
    &sineManipulationA_Exit,
    nullptr
};

static ParameterFrequency    sineA_Frequency(FREQUENCY_SINE_MIN, FREQUENCY_SINE_MAX);
static ParameterAmplitude    sineA_Amplitude;
static ParameterOffset       sineA_Offset;
static ParameterManipulation sineA_Manipulation(sineManipulationA);

static Parameter *params_SineA[] =
{
    &sineA_Frequency,
    &sineA_Amplitude,
    &sineA_Offset,
    &sineA_Manipulation,
    nullptr
};

static Form formSineA(TypeForm::Sine, params_SineA, &waves[Chan::A]);


static pchar namesModeStartFree[] =       // Режимы запуска для произвольных сигналов
{
    "Авто",   "Auto",
    "Однокр", "Single",
    "Комп А", "Comp A",
    "Форм B", "Form B",
    nullptr
};

static pchar namesModeStartPulse[] =   // Режимы запуска для мпульсных сигналов
{
    "Авто",   "Auto",
    "Однокр", "Single",
    nullptr
};


static ParameterFrequency  rampPlusA_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParameterAmplitude  rampPlusA_Amplitude;
static ParameterOffset     rampPlusA_Offset;
static ParameterModeStart  rampPlusA_ModeStart(namesModeStartFree);

static Parameter *params_RampPlusA[] =
{
    &rampPlusA_Frequency,
    &rampPlusA_Amplitude,
    &rampPlusA_Offset,
    &rampPlusA_ModeStart,
    nullptr
};

static Form formRampPlusA(TypeForm::RampPlus, params_RampPlusA, &waves[Chan::A]);


static ParameterFrequency  rampMinusA_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParameterAmplitude  rampMinusA_Amplitude;
static ParameterOffset     rampMinusA_Offset;
static ParameterModeStart  rampMinusA_ModeStart(namesModeStartFree);

static Parameter *params_RampMinusA[] =
{
    &rampMinusA_Frequency,
    &rampMinusA_Amplitude,
    &rampMinusA_Offset,
    &rampMinusA_ModeStart,
    nullptr
};

static Form formRampMinusA(TypeForm::RampMinus, params_RampMinusA, &waves[Chan::A]);


static ParameterFrequency triangleA_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParameterAmplitude triangleA_Amplitude;
static ParameterOffset    triangleA_Offset;
static ParameterModeStart triangleA_ModeStart(namesModeStartFree);

static Parameter *params_TriangleA[] =
{
    &triangleA_Frequency,
    &triangleA_Amplitude,
    &triangleA_Offset,
    &triangleA_ModeStart,
    nullptr
};

static Form formTriangleA(TypeForm::Triangle, params_TriangleA, &waves[Chan::A]);


static void OnChoose_FileA()
{
    PageLoadForm::LoadForm(ChA);
}

static ParameterFrequency  freeA_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParameterAmplitude  freeA_Amplitude;
static ParameterOffset     freeA_Offset;
static ParameterModeStart  freeA_ModeStart(namesModeStartFree);
static ParameterButton     freeA_Choose   ("Выбрать", "Choose", OnChoose_FileA);

static Parameter *params_FreeA[] =
{
    &freeA_Frequency,
    &freeA_Amplitude,
    &freeA_Offset,
    &freeA_ModeStart,
    &freeA_Choose,
    nullptr
};

static Form formFreeA(TypeForm::Free, params_FreeA, &waves[Chan::A]);



static ParameterFrequency meanderA_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParameterAmplitude meanderA_Amplitude;
static ParameterOffset    meanderA_Offset;

static Parameter *params_MeanderA[] =
{
    &meanderA_Frequency,
    &meanderA_Amplitude,
    &meanderA_Offset,
    nullptr
};

static Form formMeanderA(TypeForm::Meander, params_MeanderA, &waves[Chan::A]);


static pchar namesPolarity[] =
{
    POLARITY_POS, POLARITY_POS,
    POLARITY_NEG, POLARITY_NEG,
    nullptr
};


static bool FuncActive_PeriodImpulseA()
{
    return (impulseA_ModeStart.GetChoice() == 0);
}

static ParameterPeriod    impulseA_Period   (FuncActive_PeriodImpulseA, Value("100", Order::Kilo), Value("100", Order::Micro));
static ParameterDuration  impulseA_Duration (Value("100", Order::Kilo), Value("20", Order::Micro));
static ParameterAmplitude impulseA_Amplitude;
static ParameterOffset    impulseA_Offset;
static ParameterPolarity  impulseA_Polarity(namesPolarity);
static ParameterModeStart impulseA_ModeStart(namesModeStartPulse);

static Parameter *params_ImpulseA[] =
{
    &impulseA_Period,
    &impulseA_Duration,
    &impulseA_Amplitude,
    &impulseA_Offset,
    &impulseA_Polarity,
    &impulseA_ModeStart,
    nullptr
};

static Form formImpulseA(TypeForm::Impulse, params_ImpulseA, &waves[Chan::A]);


static bool FuncActive_PeriodPacketA()
{
    return true;
}

static ParameterPeriod       packetA_Period        (FuncActive_PeriodPacketA, Value("100", Order::Kilo), Value("200", Order::Micro),
    "Период имп", "Period imp");
static ParameterDuration     packetA_Duration      (Value("100", Order::Kilo), Value("10", Order::Micro),  "Длит имп",
    "Dur imp");
static ParameterInteger      packetA_PacketNumber  (ParameterIntegerType::PacketNumber,
    "Кол-во имп", "Count imp", Value("1", Order::One), Value("1000000000", Order::One), Value("3", Order::One));
static ParameterPacketPeriod packetA_PacketPeriod  (Value("100", Order::Kilo), Value("0.1", Order::One));
static ParameterAmplitude    packetA_Amplitude;
static ParameterOffset       packetA_Offset;
static ParameterPolarity     packetA_Polarity(namesPolarity);
static ParameterModeStart    packetA_ModeStart(namesModeStartPulse);

static Parameter *params_PacketA[] =
{
    &packetA_Period,
    &packetA_Duration,
    &packetA_PacketNumber,
    &packetA_PacketPeriod,
    &packetA_Amplitude,
    &packetA_Offset,
    &packetA_Polarity,
    &packetA_ModeStart,
    nullptr
};

static Form formPacketImpulseA(TypeForm::Packet, params_PacketA, &waves[Chan::A]);



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
    &formFreeA,
    nullptr
};



static ParameterManipulationEnabled  sineManipulationB_Enabled  (namesManipulationEnabled);
static ParameterManipulationDuration sineManipulationB_Duration(Value("10", Order::Nano), Value("10", Order::One),
    Value("5", Order::Milli));
static ParameterManipulationPeriod   sineManipulationB_Period(Value("20", Order::Nano), Value("10000", Order::One),
    Value("25", Order::Milli));
static ParameterButton               sineManipulationB_Exit     ("Закрыть ( ESC )", "Close ( ESC )", FuncCloseManipulation);

static Parameter *sineManipulationB[] =
{
    &sineManipulationB_Enabled,
    &sineManipulationB_Duration,
    &sineManipulationB_Period,
    &sineManipulationB_Exit,
    nullptr
};

static ParameterFrequency    sineB_Frequency(FREQUENCY_SINE_MIN, FREQUENCY_SINE_MAX);
static ParameterAmplitude    sineB_Amplitude;
static ParameterOffset       sineB_Offset;
static ParameterPhase        sineB_Phase;
static ParameterManipulation sineB_Manipulation(sineManipulationB);

static Parameter *params_SineB[] =
{
    &sineB_Frequency,
    &sineB_Amplitude,
    &sineB_Offset,
    &sineB_Phase,
    &sineB_Manipulation,
    nullptr
};

static Form formSineB(TypeForm::Sine, params_SineB, &waves[Chan::B]);


static ParameterFrequency  rampPlusB_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParameterAmplitude  rampPlusB_Amplitude;
static ParameterOffset     rampPlusB_Offset;
static ParameterModeStart  rampPlusB_ModeStart(namesModeStartFree);

static Parameter *params_RampPlusB[] =
{
    &rampPlusB_Frequency,
    &rampPlusB_Amplitude,
    &rampPlusB_Offset,
    &rampPlusB_ModeStart,
    nullptr
};

static Form formRampPlusB(TypeForm::RampPlus, params_RampPlusB, &waves[Chan::B]);


static ParameterFrequency  rampMinusB_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParameterAmplitude  rampMinusB_Amplitude;
static ParameterOffset     rampMinusB_Offset;
static ParameterModeStart  rampMinusB_ModeStart(namesModeStartFree);

static Parameter *params_RampMinusB[] =
{
    &rampMinusB_Frequency,
    &rampMinusB_Amplitude,
    &rampMinusB_Offset,
    &rampMinusB_ModeStart,
    nullptr
};

static Form formRampMinusB(TypeForm::RampMinus, params_RampMinusB, &waves[Chan::B]);


static ParameterFrequency  triangleB_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParameterAmplitude  triangleB_Amplitude;
static ParameterOffset     triangleB_Offset;
static ParameterModeStart  triangleB_ModeStart(namesModeStartFree);

static Parameter *params_TriangleB[] =
{
    &triangleB_Frequency,
    &triangleB_Amplitude,
    &triangleB_Offset,
    &triangleB_ModeStart,
    nullptr
};

static Form formTriangleB(TypeForm::Triangle, params_TriangleB, &waves[Chan::B]);


static void OnChoose_FileB()
{
    PageLoadForm::LoadForm(ChB);
}

static ParameterFrequency  freeB_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParameterAmplitude  freeB_Amplitude;
static ParameterOffset     freeB_Offset;
static ParameterModeStart  freeB_ModeStart(namesModeStartFree);
static ParameterButton     freeB_Choose("Выбрать", "Choose", OnChoose_FileB);

static Parameter *params_FreeB[] =
{
    &freeB_Frequency,
    &freeB_Amplitude,
    &freeB_Offset,
    &freeB_ModeStart,
    &freeB_Choose,
    nullptr
};

static Form formFreeB(TypeForm::Free, params_FreeB, &waves[Chan::B]);


static ParameterFrequency meanderB_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParameterAmplitude meanderB_Amplitude;
static ParameterOffset    meanderB_Offset;

static Parameter *params_MeanderB[] =
{
    &meanderB_Frequency,
    &meanderB_Amplitude,
    &meanderB_Offset,
    nullptr
};

static Form formMeanderB(TypeForm::Meander, params_MeanderB, &waves[Chan::B]);


static bool FuncActive_PeriodImpulseB()
{
    return true;
}

static ParameterPeriod    impulseB_Period   (FuncActive_PeriodImpulseB, Value("100", Order::Kilo), Value("100", Order::Micro));
static ParameterDuration  impulseB_Duration (Value("100", Order::Kilo), Value("20", Order::Micro));
static ParameterAmplitude impulseB_Amplitude;
static ParameterOffset    impulseB_Offset;
static ParameterPolarity  impulseB_Polarity(namesPolarity);
static ParameterModeStart impulseB_ModeStart(namesModeStartPulse);

static Parameter *params_ImpulseB[] =
{
    &impulseB_Period,
    &impulseB_Duration,
    &impulseB_Amplitude,
    &impulseB_Offset,
    &impulseB_Polarity,
    &impulseB_ModeStart,
    nullptr
};

static Form formImpulseB(TypeForm::Impulse, params_ImpulseB, &waves[Chan::B]);



extern Wave waves[Chan::Count];

static Form *formsB[] =
{
    &formSineB,
    &formRampPlusB,
    &formRampMinusB,
    &formTriangleB,
    &formMeanderB,
    &formImpulseB,
    &formFreeB,
    nullptr
};



Wave waves[Chan::Count] =
{
    Wave(Chan::A, formsA),
    Wave(Chan::B, formsB)
};


Form *Signals::A::impulse = &formImpulseA;
Form *Signals::A::packet = &formPacketImpulseA;
Form *Signals::B::impulse = &formImpulseB;
