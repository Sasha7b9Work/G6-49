#include "defines.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Generator/Signals.h"
#include "Generator/Parameters.h"
#include "Menu/Pages/Pages.h"


static const char *namesModulationEnabled[] =
{
    DISABLED_RU, DISABLED_EN,
    ENABLED_RU,  ENABLED_EN,
    nullptr
};

static ParameterManipulationEnabled   sineModulationA_Manipulation        (namesModulationEnabled);
static ParameterManipulationDuration  sineModulationA_ManipulationDuration(Value("0"), Value("10"),    Value("5"));
static ParameterManipulationPeriod    sineModulationA_ManipulationPeriod  (Value("0"), Value("10000"), Value("0.002500"));
//static ParameterDouble  SineModulationA_Exit                (ParameterDouble::Exit,                 -1.0F, 1.0F,   "",     0, Order::One);


static Parameter *params_SineModulationA[] =
{
    &sineModulationA_Manipulation,
    &sineModulationA_ManipulationDuration,
    &sineModulationA_ManipulationPeriod,
//    &SineModulationA_Exit,
    0
};

static ParameterFrequency    sineA_Frequency;
static ParameterAmplitude    sineA_Amplitude;
static ParameterOffset       sineA_Offset;
static ParameterManipulation sineA_Manipulation(params_SineModulationA);

static Parameter *params_SineA[] =
{
    &sineA_Frequency,
    &sineA_Amplitude,
    &sineA_Offset,
    &sineA_Manipulation,
    0
};

static Form formSineA(TypeForm::Sine, params_SineA, &waves[Chan::A]);


static const char *namesModeStart[] =
{
    " Авто",   " Auto",
    " Однокр", " Single",
    " Комп А", " Comp A",
    " Форм B", " Form B",
    nullptr
};


static ParameterFrequency  rampPlusA_Frequency;
static ParameterAmplitude  rampPlusA_Amplitude;
static ParameterOffset     rampPlusA_Offset;
static ParameterModeStart  rampPlusA_ModeStart(namesModeStart);

static Parameter *params_RampPlusA[] =
{
    &rampPlusA_Frequency,
    &rampPlusA_Amplitude,
    &rampPlusA_Offset,
    &rampPlusA_ModeStart,
    0
};

static Form formRampPlusA(TypeForm::RampPlus, params_RampPlusA, &waves[Chan::A]);


static ParameterFrequency  rampMinusA_Frequency;
static ParameterAmplitude  rampMinusA_Amplitude;
static ParameterOffset     rampMinusA_Offset;
static ParameterModeStart  rampMinusA_ModeStart(namesModeStart);

static Parameter *params_RampMinusA[] =
{
    &rampMinusA_Frequency,
    &rampMinusA_Amplitude,
    &rampMinusA_Offset,
    &rampMinusA_ModeStart,
    0
};

static Form formRampMinusA(TypeForm::RampMinus, params_RampMinusA, &waves[Chan::A]);


static ParameterFrequency triangleA_Frequency;
static ParameterAmplitude triangleA_Amplitude;
static ParameterOffset    triangleA_Offset;
static ParameterModeStart triangleA_ModeStart(namesModeStart);

static Parameter *params_TriangleA[] =
{
    &triangleA_Frequency,
    &triangleA_Amplitude,
    &triangleA_Offset,
    &triangleA_ModeStart,
    0
};

static Form formTriangleA(TypeForm::Triangle, params_TriangleA, &waves[Chan::A]);


static ParameterFrequency  freeA_Frequency;
static ParameterAmplitude  freeA_Amplitude;
static ParameterOffset     freeA_Offset;
static ParameterModeStart  freeA_ModeStart(namesModeStart);
//static ParameterPage   FreeA_Choice   (ParameterPage::ChoiceForm, reinterpret_cast<PageBase *>(PageLoadForm::pointer));

static Parameter *params_FreeA[] =
{
    &freeA_Frequency,
    &freeA_Amplitude,
    &freeA_Offset,
    &freeA_ModeStart,
//    &FreeA_Choice,
    0
};

static Form formFreeA(TypeForm::Free, params_FreeA, &waves[Chan::A]);



static ParameterFrequency meanderA_Frequency;
static ParameterAmplitude meanderA_Amplitude;
static ParameterOffset    meanderA_Offset;

static Parameter *params_MeanderA[] =
{
    &meanderA_Frequency,
    &meanderA_Amplitude,
    &meanderA_Offset,
    0
};

static Form formMeanderA(TypeForm::Meander, params_MeanderA, &waves[Chan::A]);


static const char *namesPolarity[] =
{
    POLARITY_POS, POLARITY_POS,
    POLARITY_NEG, POLARITY_NEG,
    nullptr
};


static ParameterPeriod    impulseA_Period   (Value("33", Order::Nano), Value("100", Order::Kilo), Value("1"));
static ParameterDuration  impulseA_Duration (Value("0"),               Value("100", Order::Mega), Value("100", Order::Micro));
static ParameterAmplitude impulseA_Amplitude;
static ParameterOffset    impulseA_Offset;
static ParameterPolarity  impulseA_Polarity(namesPolarity);
static ParameterModeStart impulseA_ModeStart(namesModeStart);

static Parameter *params_ImpulseA[] =
{
    &impulseA_Period,
    &impulseA_Duration,
    &impulseA_Amplitude,
    &impulseA_Offset,
    &impulseA_Polarity,
    &impulseA_ModeStart,
    0
};

static Form formImpulseA(TypeForm::Impulse, params_ImpulseA, &waves[Chan::A]);



static ParameterPeriod       packetA_Period        (Value("0"), Value("100", Order::Mega), Value("200", Order::Micro));
static ParameterDuration     packetA_Duration      (Value("0"), Value("100", Order::Mega), Value("100", Order::Micro));
static ParameterDouble       packetA_PacketNumber (ParameterDoubleType::PacketNumber, "Кол-во имп", "Count imp", Value("0"), Value("100", Order::Mega), Value("3"));
static ParameterPacketPeriod packetA_PacketPeriod  (Value("0"), Value("100", Order::Mega), Value("0.1"));
static ParameterAmplitude    packetA_Amplitude;
static ParameterOffset       packetA_Offset;
static ParameterPolarity     packetA_Polarity(namesPolarity);
static ParameterModeStart    packetA_ModeStart(namesModeStart);

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
    0
};

static Form formPacketImpulseA(TypeForm::PacketImpuls, params_PacketA, &waves[Chan::A]);



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
    0
};



static ParameterManipulationEnabled  sineModulationB_Manipulation         (namesModulationEnabled);
static ParameterManipulationDuration sineModulationB_ManipulationDuration (Value("0"), Value("10", Order::Kilo), Value("0.5"));
static ParameterManipulationPeriod   sineModulationB_ManipulationPeriod   (Value("0"), Value("10", Order::Kilo), Value("2.5", Order::Milli));
//static ParameterDouble  SineModulationB_Exit                (ParameterDouble::Exit,                 -1.0F, 1.0F,   "",     0, Order::One);

static Parameter *params_SineModulationB[] =
{
    &sineModulationB_Manipulation,
    &sineModulationB_ManipulationDuration,
    &sineModulationB_ManipulationPeriod,
    0
};

static ParameterFrequency    sineB_Frequency;
static ParameterAmplitude    sineB_Amplitude;
static ParameterOffset       sineB_Offset;
static ParameterPhase        sineB_Phase;
static ParameterManipulation sineB_Manipulation(params_SineModulationB);

static Parameter *params_SineB[] =
{
    &sineB_Frequency,
    &sineB_Amplitude,
    &sineB_Offset,
    &sineB_Phase,
    &sineB_Manipulation,
    0
};

static Form formSineB(TypeForm::Sine, params_SineB, &waves[Chan::B]);


static ParameterFrequency  rampPlusB_Frequency;
static ParameterAmplitude  rampPlusB_Amplitude;
static ParameterOffset     rampPlusB_Offset;
static ParameterModeStart  rampPlusB_ModeStart(namesModeStart);

static Parameter *params_RampPlusB[] =
{
    &rampPlusB_Frequency,
    &rampPlusB_Amplitude,
    &rampPlusB_Offset,
    &rampPlusB_ModeStart,
    0
};

static Form formRampPlusB(TypeForm::RampPlus, params_RampPlusB, &waves[Chan::B]);


static ParameterFrequency  rampMinusB_Frequency;
static ParameterAmplitude  rampMinusB_Amplitude;
static ParameterOffset     rampMinusB_Offset;
static ParameterModeStart  rampMinusB_ModeStart(namesModeStart);

static Parameter *params_RampMinusB[] =
{
    &rampMinusB_Frequency,
    &rampMinusB_Amplitude,
    &rampMinusB_Offset,
    &rampMinusB_ModeStart,
    0
};

static Form formRampMinusB(TypeForm::RampMinus, params_RampMinusB, &waves[Chan::B]);


static ParameterFrequency  triangleB_Frequency;
static ParameterAmplitude  triangleB_Amplitude;
static ParameterOffset     triangleB_Offset;
static ParameterModeStart  triangleB_ModeStart(namesModeStart);

static Parameter *params_TriangleB[] =
{
    &triangleB_Frequency,
    &triangleB_Amplitude,
    &triangleB_Offset,
    &triangleB_ModeStart,
    0
};

static Form formTriangleB(TypeForm::Triangle, params_TriangleB, &waves[Chan::B]);


static ParameterFrequency  freeB_Frequency;
static ParameterAmplitude  freeB_Amplitude;
static ParameterOffset     freeB_Offset;
static ParameterModeStart  freeB_ModeStart(namesModeStart);
//static ParameterPage   FreeB_Choice   (ParameterPage::ChoiceForm, reinterpret_cast<PageBase *>(PageLoadForm::pointer));

static Parameter *params_FreeB[] =
{
    &freeB_Frequency,
    &freeB_Amplitude,
    &freeB_Offset,
    &freeB_ModeStart,
//    &FreeB_Choice,
    0
};

static Form formFreeB(TypeForm::Free, params_FreeB, &waves[Chan::B]);


static ParameterFrequency meanderB_Frequency;
static ParameterAmplitude meanderB_Amplitude;
static ParameterOffset    meanderB_Offset;

static Parameter *params_MeanderB[] =
{
    &meanderB_Frequency,
    &meanderB_Amplitude,
    &meanderB_Offset,
    0
};

static Form formMeanderB(TypeForm::Meander, params_MeanderB, &waves[Chan::B]);



static ParameterPeriod    impulseB_Period   (Value("33", Order::Nano), Value("10",  Order::Mega), Value("100", Order::Milli));
static ParameterDuration  impulseB_Duration (Value("0"),               Value("100", Order::Mega), Value("100", Order::Micro));
static ParameterAmplitude impulseB_Amplitude;
static ParameterOffset    impulseB_Offset;
static ParameterPolarity  impulseB_Polarity(namesPolarity);
static ParameterModeStart impulseB_ModeStart(namesModeStart);

static Parameter *params_ImpulseB[] =
{
    &impulseB_Period,
    &impulseB_Duration,
    &impulseB_Amplitude,
    &impulseB_Offset,
    &impulseB_Polarity,
    &impulseB_ModeStart,
    0
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
    0
};



Wave waves[Chan::Count] =
{
    Wave(Chan::A, formsA),
    Wave(Chan::B, formsB)
};
