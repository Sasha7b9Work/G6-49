#include "defines.h"
#include "Signals.h"
#include "Display/Painter.h"
#include "Generator/Parameters.h"
#include "Menu/Pages/Addition/PageLoadForm.h"
#include "Menu/Pages/PageSignals/PageSignals.h"


static ParameterManipulationEnabled   param_SineModulationA_Manipulation        (DISABLED_RU, ENABLED_RU);
static ParameterManipulationDuration  param_SineModulationA_ManipulationDuration(FloatValue(0, 0), FloatValue(10,    0), FloatValue(5, 0));
static ParameterManipulationPeriod    param_SineModulationA_ManipulationPeriod  (FloatValue(0, 0), FloatValue(10000, 0), FloatValue(0, 2, 500));
//static ParameterValue  param_SineModulationA_Exit                (ParameterValue::Exit,                 -1.0F, 1.0F,   "",     0, Order::One);


static Parameter *params_SineModulationA[] =
{
    &param_SineModulationA_Manipulation,
    &param_SineModulationA_ManipulationDuration,
    &param_SineModulationA_ManipulationPeriod,
//    &param_SineModulationA_Exit,
    0
};

static ParameterFrequency    param_SineA_Frequency;
static ParameterAmplitude    param_SineA_Amplitude;
static ParameterOffset       param_SineA_Offset;
static ParameterManipulation param_SineA_Manipulation(params_SineModulationA);

static Parameter *params_SineA[] =
{
    &param_SineA_Frequency,
    &param_SineA_Amplitude,
    &param_SineA_Offset,
    &param_SineA_Manipulation,
    0
};

static Form formSineA(TypeForm::Sine, params_SineA, &waves[Chan::A]);


static ParameterFrequency  param_RampPlusA_Frequency;
static ParameterAmplitude  param_RampPlusA_Amplitude;
static ParameterOffset     param_RampPlusA_Offset;
static ParameterModeStart  param_RampPlusA_ModeStart;

static Parameter *params_RampPlusA[] =
{
    &param_RampPlusA_Frequency,
    &param_RampPlusA_Amplitude,
    &param_RampPlusA_Offset,
    &param_RampPlusA_ModeStart,
    0
};

static Form formRampPlusA(TypeForm::RampPlus, params_RampPlusA, &waves[Chan::A]);


static ParameterFrequency  param_RampMinusA_Frequency;
static ParameterAmplitude  param_RampMinusA_Amplitude;
static ParameterOffset     param_RampMinusA_Offset;
static ParameterModeStart  param_RampMinusA_ModeStart;

static Parameter *params_RampMinusA[] =
{
    &param_RampMinusA_Frequency,
    &param_RampMinusA_Amplitude,
    &param_RampMinusA_Offset,
    &param_RampMinusA_ModeStart,
    0
};

static Form formRampMinusA(TypeForm::RampMinus, params_RampMinusA, &waves[Chan::A]);


static ParameterFrequency param_TriangleA_Frequency;
static ParameterAmplitude param_TriangleA_Amplitude;
static ParameterOffset    param_TriangleA_Offset;
static ParameterModeStart param_TriangleA_ModeStart;

static Parameter *params_TriangleA[] =
{
    &param_TriangleA_Frequency,
    &param_TriangleA_Amplitude,
    &param_TriangleA_Offset,
    &param_TriangleA_ModeStart,
    0
};

static Form formTriangleA(TypeForm::Triangle, params_TriangleA, &waves[Chan::A]);


static ParameterFrequency  param_FreeA_Frequency;
static ParameterAmplitude  param_FreeA_Amplitude;
static ParameterOffset     param_FreeA_Offset;
static ParameterModeStart  param_FreeA_ModeStart;
//static ParameterPage   param_FreeA_Choice   (ParameterPage::ChoiceForm, reinterpret_cast<PageBase *>(PageLoadForm::pointer));

static Parameter *params_FreeA[] =
{
    &param_FreeA_Frequency,
    &param_FreeA_Amplitude,
    &param_FreeA_Offset,
    &param_FreeA_ModeStart,
//    &param_FreeA_Choice,
    0
};

static Form formFreeA(TypeForm::Free, params_FreeA, &waves[Chan::A]);



static ParameterFrequency param_MeanderA_Frequency;
static ParameterAmplitude param_MeanderA_Amplitude;
static ParameterOffset    param_MeanderA_Offset;

static Parameter *params_MeanderA[] =
{
    &param_MeanderA_Frequency,
    &param_MeanderA_Amplitude,
    &param_MeanderA_Offset,
    0
};

static Form formMeanderA(TypeForm::Meander, params_MeanderA, &waves[Chan::A]);


static ParameterPeriod    param_ImpulseA_Period   (FloatValue(0, 0, 0, 33), FloatValue(10 * 1000 * 1000,  0), FloatValue(0, 1));
static ParameterDuration  param_ImpulseA_Duration (FloatValue(0, 0),        FloatValue(100 * 1000 * 1000, 0), FloatValue(0, 0, 100));
static ParameterAmplitude param_ImpulseA_Amplitude;
static ParameterOffset    param_ImpulseA_Offset;
static ParameterPolarity  param_ImpulseA_Polarity;
static ParameterModeStart param_ImpulseA_ModeStart;

static Parameter *params_ImpulseA[] =
{
    &param_ImpulseA_Period,
    &param_ImpulseA_Duration,
    &param_ImpulseA_Amplitude,
    &param_ImpulseA_Offset,
    &param_ImpulseA_Polarity,
    &param_ImpulseA_ModeStart,
    0
};

static Form formImpulseA(TypeForm::Impulse, params_ImpulseA, &waves[Chan::A]);



static ParameterPeriod       param_PacketA_Period       (FloatValue(0, 0), FloatValue(100 * 1000 * 1000, 0), FloatValue(0, 0, 200));
static ParameterDuration     param_PacketA_Duration     (FloatValue(0, 0), FloatValue(100 * 1000 * 1000, 0), FloatValue(0, 0, 100));
static ParameterValue        param_PacketA_PacketNumber (ParameterValue::PacketNumber, "Кол-во имп", FloatValue(0, 0), FloatValue(100 * 1000 * 1000, 0), FloatValue(3, 0));
static ParameterPacketPeriod param_PacketA_PacketPeriod (FloatValue(0, 0), FloatValue(100 * 1000 * 1000, 0), FloatValue(0, 1));
static ParameterAmplitude    param_PacketA_Amplitude;
static ParameterOffset       param_PacketA_Offset;
static ParameterPolarity     param_PacketA_Polarity;
static ParameterModeStart    param_PacketA_ModeStart;

static Parameter *params_PacketA[] =
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



static ParameterManipulationEnabled  param_SineModulationB_Manipulation         (DISABLED_RU, ENABLED_RU);
static ParameterManipulationDuration param_SineModulationB_ManipulationDuration (FloatValue(0, 0), FloatValue(10 * 1000, 0), FloatValue(0, 5));
static ParameterManipulationPeriod   param_SineModulationB_ManipulationPeriod   (FloatValue(0, 0), FloatValue(10 * 1000, 0), FloatValue(0, 0, 2500));
//static ParameterValue  param_SineModulationB_Exit                (ParameterValue::Exit,                 -1.0F, 1.0F,   "",     0, Order::One);

static Parameter *params_SineModulationB[] =
{
    &param_SineModulationB_Manipulation,
    &param_SineModulationB_ManipulationDuration,
    &param_SineModulationB_ManipulationPeriod,
    0
};

static ParameterFrequency    param_SineB_Frequency;
static ParameterAmplitude    param_SineB_Amplitude;
static ParameterOffset       param_SineB_Offset;
static ParameterPhase        param_SineB_Phase;
static ParameterManipulation param_SineB_Manipulation(params_SineModulationB);

static Parameter *params_SineB[] =
{
    &param_SineB_Frequency,
    &param_SineB_Amplitude,
    &param_SineB_Offset,
    &param_SineB_Phase,
    &param_SineB_Manipulation,
    0
};

static Form formSineB(TypeForm::Sine, params_SineB, &waves[Chan::B]);


static ParameterFrequency  param_RampPlusB_Frequency;
static ParameterAmplitude  param_RampPlusB_Amplitude;
static ParameterOffset     param_RampPlusB_Offset;
static ParameterModeStart  param_RampPlusB_ModeStart;

static Parameter *params_RampPlusB[] =
{
    &param_RampPlusB_Frequency,
    &param_RampPlusB_Amplitude,
    &param_RampPlusB_Offset,
    &param_RampPlusB_ModeStart,
    0
};

static Form formRampPlusB(TypeForm::RampPlus, params_RampPlusB, &waves[Chan::B]);


static ParameterFrequency  param_RampMinusB_Frequency;
static ParameterAmplitude  param_RampMinusB_Amplitude;
static ParameterOffset     param_RampMinusB_Offset;
static ParameterModeStart  param_RampMinusB_ModeStart;

static Parameter *params_RampMinusB[] =
{
    &param_RampMinusB_Frequency,
    &param_RampMinusB_Amplitude,
    &param_RampMinusB_Offset,
    &param_RampMinusB_ModeStart,
    0
};

static Form formRampMinusB(TypeForm::RampMinus, params_RampMinusB, &waves[Chan::B]);


static ParameterFrequency  param_TriangleB_Frequency;
static ParameterAmplitude  param_TriangleB_Amplitude;
static ParameterOffset     param_TriangleB_Offset;
static ParameterModeStart  param_TriangleB_ModeStart;

static Parameter *params_TriangleB[] =
{
    &param_TriangleB_Frequency,
    &param_TriangleB_Amplitude,
    &param_TriangleB_Offset,
    &param_TriangleB_ModeStart,
    0
};

static Form formTriangleB(TypeForm::Triangle, params_TriangleB, &waves[Chan::B]);


static ParameterFrequency  param_FreeB_Frequency;
static ParameterAmplitude  param_FreeB_Amplitude;
static ParameterOffset     param_FreeB_Offset;
static ParameterModeStart  param_FreeB_ModeStart;
//static ParameterPage   param_FreeB_Choice   (ParameterPage::ChoiceForm, reinterpret_cast<PageBase *>(PageLoadForm::pointer));

static Parameter *params_FreeB[] =
{
    &param_FreeB_Frequency,
    &param_FreeB_Amplitude,
    &param_FreeB_Offset,
    &param_FreeB_ModeStart,
//    &param_FreeB_Choice,
    0
};

static Form formFreeB(TypeForm::Free, params_FreeB, &waves[Chan::B]);


static ParameterFrequency param_MeanderB_Frequency;
static ParameterAmplitude param_MeanderB_Amplitude;
static ParameterOffset    param_MeanderB_Offset;

static Parameter *params_MeanderB[] =
{
    &param_MeanderB_Frequency,
    &param_MeanderB_Amplitude,
    &param_MeanderB_Offset,
    0
};

static Form formMeanderB(TypeForm::Meander, params_MeanderB, &waves[Chan::B]);



static ParameterPeriod    param_ImpulseB_Period   (FloatValue(0, 0, 0, 33), FloatValue(10 * 1000 * 1000, 0), FloatValue(0, 1));
static ParameterDuration  param_ImpulseB_Duration (FloatValue(0, 0), FloatValue(100 * 1000 * 1000, 0), FloatValue(0, 0, 100));
static ParameterAmplitude param_ImpulseB_Amplitude;
static ParameterOffset    param_ImpulseB_Offset;
static ParameterPolarity  param_ImpulseB_Polarity;
static ParameterModeStart param_ImpulseB_ModeStart;

static Parameter *params_ImpulseB[] =
{
    &param_ImpulseB_Period,
    &param_ImpulseB_Duration,
    &param_ImpulseB_Amplitude,
    &param_ImpulseB_Offset,
    &param_ImpulseB_Polarity,
    &param_ImpulseB_ModeStart,
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
