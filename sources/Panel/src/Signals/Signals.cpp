// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Signals/Signals.h"
#include "Signals/Parameters.h"
#include "Menu/Pages/Pages.h"


extern PModeStart impulseA_ModeStart;
extern PModeStart packetA_ModeStart;
extern PModeStart impulseB_ModeStart;

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


static PManipulationEnabled   sineManipulationA_Enabled (namesManipulationEnabled);
static PManipulationDuration  sineManipulationA_Duration(Value("10", Order::Nano), Value("10", Order::One),
                                                                 Param::EValueInRange,
                                                                 Value("5", Order::Milli));
static PManipulationPeriod    sineManipulationA_Period  (Value("20", Order::Nano), Value("10000", Order::One),
                                                                 Param::EValueInRange,
                                                                 Value("25", Order::Milli));
static BParam                sineManipulationA_Exit    ("Закрыть ( ESC )", "Close ( ESC )", FuncCloseManipulation);


static Param *sineManipulationA[] =
{
    &sineManipulationA_Enabled,
    &sineManipulationA_Duration,
    &sineManipulationA_Period,
    &sineManipulationA_Exit,
    nullptr
};


static SMinMax OffstInRange(Form *form)
{
    // Ампл == 0  | [0 ... 5]
    // Ампл <= 1В | [0 ... 2.5], ampl / 2 + fabs(см) <= 2.5
    // Ампл > 1В  | [0 ... 5],   ампл / 2 + fabs(см) <= 5

    Value amplitude = form->FindParameter(TypeDParam::Amplitude)->GetValue();
    DParam *param_offset = form->FindParameter(TypeDParam::Offset);

    SMinMax result;
    result.max = param_offset->Max();

    if (amplitude.Abs() == 0)
    {
        result.max = param_offset->Max();
    }
    else if(amplitude.ToDouble() <= 1.0F)
    {
        result.max.Div(2);
    }

    amplitude.Div(2);

    result.max.Sub(amplitude);

    result.min = result.max;
    result.min.SetSign(-1);

    Value offset = param_offset->GetValue();

    result.valid = (offset <= result.max) && (offset >= result.min);

    return result;
}

static PFrequency     sineA_Frequency(FREQUENCY_SINE_MIN, FREQUENCY_SINE_MAX);
static ParamAmplitude sineA_Amplitude;
static ParamOffset    sineA_Offset(OffstInRange);
static PManipulation  sineA_Manipulation(sineManipulationA);

static Param *params_SineA[] =
{
    &sineA_Frequency,
    &sineA_Amplitude,
    &sineA_Offset,
    &sineA_Manipulation,
    nullptr
};

static Form formSineA(TypeForm::Sine, params_SineA, &waves[Chan::A]);


static pchar namesModeStartFree[] =         // Режимы запуска для произвольных сигналов
{
    "Авто",   "Auto",
    "Однокр", "Single",
    "Комп А", "Comp A",
    "Форм B", "Form B",
    nullptr
};

static pchar namesModeStartImpulse[] =      // Режимы запуска для мпульсных сигналов
{
    "Авто",   "Auto",
    "Однокр", "Single",
    nullptr
};

static pchar namesModeStartStopImpulse[] =  // Включение режима "А-Старт,В-Стоп"
{
    "Откл", "Disable",
    "Вкл",  "Enable",
    nullptr
};


static pchar namesClockImpulse[] =
{
    "100 МГц", "100 MHz",
    "1 МГц", "1 MHz",
    nullptr
};


static PFrequency  rampPlusA_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParamAmplitude  rampPlusA_Amplitude;
static ParamOffset     rampPlusA_Offset;
static PModeStart  rampPlusA_ModeStart(Param::EFuncActive, namesModeStartFree);

static Param *params_RampPlusA[] =
{
    &rampPlusA_Frequency,
    &rampPlusA_Amplitude,
    &rampPlusA_Offset,
    &rampPlusA_ModeStart,
    nullptr
};

static Form formRampPlusA(TypeForm::RampPlus, params_RampPlusA, &waves[Chan::A]);


static PFrequency  rampMinusA_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParamAmplitude  rampMinusA_Amplitude;
static ParamOffset     rampMinusA_Offset;
static PModeStart  rampMinusA_ModeStart(Param::EFuncActive, namesModeStartFree);

static Param *params_RampMinusA[] =
{
    &rampMinusA_Frequency,
    &rampMinusA_Amplitude,
    &rampMinusA_Offset,
    &rampMinusA_ModeStart,
    nullptr
};

static Form formRampMinusA(TypeForm::RampMinus, params_RampMinusA, &waves[Chan::A]);


static PFrequency triangleA_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParamAmplitude triangleA_Amplitude;
static ParamOffset    triangleA_Offset;
static PModeStart triangleA_ModeStart(Param::EFuncActive, namesModeStartFree);

static Param *params_TriangleA[] =
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

static PFrequency  freeA_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParamAmplitude  freeA_Amplitude;
static ParamOffset     freeA_Offset;
static PModeStart  freeA_ModeStart(Param::EFuncActive, namesModeStartFree);
static BParam     freeA_Choose   ("Выбрать", "Choose", OnChoose_FileA);

static Param *params_FreeA[] =
{
    &freeA_Frequency,
    &freeA_Amplitude,
    &freeA_Offset,
    &freeA_ModeStart,
    &freeA_Choose,
    nullptr
};

static Form formFreeA(TypeForm::Free, params_FreeA, &waves[Chan::A]);



static PFrequency meanderA_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParamAmplitude meanderA_Amplitude;
static ParamOffset    meanderA_Offset;

static Param *params_MeanderA[] =
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


static bool FuncActive_ModeStartStopAndDelayImpulseA()
{
    return (WAVE_B.GetCurrentForm() == B::Impulse::form);
}


static PPeriod        impulseA_Period   (FuncActive_PeriodImpulseA, Value("100", Order::Kilo), Value("100", Order::Micro));
static PDuration      impulseA_Duration (Value("100", Order::Kilo), Value("20", Order::Micro));
static ParamAmplitude     impulseA_Amplitude;
static ParamOffset        impulseA_Offset;
static PPolarity      impulseA_Polarity(namesPolarity);
static PModeStart     impulseA_ModeStart(Param::EFuncActive, namesModeStartImpulse);
static PClockImpulse  impulse_Clock(namesClockImpulse);
static PModeStartStop impulseA_StartStop(FuncActive_ModeStartStopAndDelayImpulseA, namesModeStartStopImpulse);
static PDelay         impulseA_Delay(FuncActive_ModeStartStopAndDelayImpulseA, Value("100", Order::Kilo), Value("100", Order::Micro));

PPeriod        *A::Impulse::period     = &impulseA_Period;
PDelay         *A::Impulse::delay      = &impulseA_Delay;
PModeStartStop *A::Impulse::start_stop = &impulseA_StartStop;
PModeStart     *A::Impulse::mode_start = &impulseA_ModeStart;

static Param *params_ImpulseA[] =
{
    &impulseA_Period,
    &impulseA_Duration,
    &impulseA_Amplitude,
    &impulseA_Offset,
    &impulseA_Polarity,
    &impulseA_ModeStart,
    &impulse_Clock,
    &impulseA_StartStop,
    &impulseA_Delay,
    nullptr
};

static Form formImpulseA(TypeForm::Impulse, params_ImpulseA, &waves[Chan::A]);


static bool FuncActive_PeriodPacketA()
{
    return (packetA_ModeStart.GetChoice() == 0);
}

static PPeriod       packetA_Period        (FuncActive_PeriodPacketA, Value("100", Order::Kilo), Value("200", Order::Micro),
    "Период имп", "Period imp");
static PDuration     packetA_Duration      (Value("100", Order::Kilo), Value("10", Order::Micro),  "Длит имп",
    "Dur imp");
static IParam      packetA_PacketNumber  (TypeIParam::PacketNumber, "Кол-во имп", "Count imp",
                                                    Value("1", Order::One),
                                                    Value("1000000000", Order::One),
                                                    Param::EValueInRange,
                                                    Value("3", Order::One));
static PPacketPeriod packetA_PacketPeriod  (Value("100", Order::Kilo), Value("0.1", Order::One));
static ParamAmplitude    packetA_Amplitude;
static ParamOffset       packetA_Offset;
static PPolarity     packetA_Polarity(namesPolarity);
static PModeStart    packetA_ModeStart(Param::EFuncActive, namesModeStartImpulse);

PPeriod *A::Packet::period_impulse = &packetA_Period;
PDuration *A::Packet::duration = &packetA_Duration;
IParam *A::Packet::number = &packetA_PacketNumber;

static Param *params_PacketA[] =
{
    &packetA_Period,
    &packetA_Duration,
    &packetA_PacketNumber,
    &packetA_PacketPeriod,
    &packetA_Amplitude,
    &packetA_Offset,
    &packetA_Polarity,
    &packetA_ModeStart,
    &impulse_Clock,
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



static PManipulationEnabled  sineManipulationB_Enabled  (namesManipulationEnabled);
static PManipulationDuration sineManipulationB_Duration(Value("10", Order::Nano), Value("10", Order::One),
                                                        Param::EValueInRange,
                                                        Value("5", Order::Milli));
static PManipulationPeriod   sineManipulationB_Period(Value("20", Order::Nano), Value("10000", Order::One),
                                                     Param::EValueInRange,
                                                     Value("25", Order::Milli));
static BParam               sineManipulationB_Exit     ("Закрыть ( ESC )", "Close ( ESC )", FuncCloseManipulation);

static Param *sineManipulationB[] =
{
    &sineManipulationB_Enabled,
    &sineManipulationB_Duration,
    &sineManipulationB_Period,
    &sineManipulationB_Exit,
    nullptr
};

static PFrequency    sineB_Frequency(FREQUENCY_SINE_MIN, FREQUENCY_SINE_MAX);
static ParamAmplitude    sineB_Amplitude;
static ParamOffset       sineB_Offset;
static PPhase        sineB_Phase;
static PManipulation sineB_Manipulation(sineManipulationB);

static Param *params_SineB[] =
{
    &sineB_Frequency,
    &sineB_Amplitude,
    &sineB_Offset,
    &sineB_Phase,
    &sineB_Manipulation,
    nullptr
};

static Form formSineB(TypeForm::Sine, params_SineB, &waves[Chan::B]);


static PFrequency  rampPlusB_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParamAmplitude  rampPlusB_Amplitude;
static ParamOffset     rampPlusB_Offset;
static PModeStart  rampPlusB_ModeStart(Param::EFuncActive, namesModeStartFree);

static Param *params_RampPlusB[] =
{
    &rampPlusB_Frequency,
    &rampPlusB_Amplitude,
    &rampPlusB_Offset,
    &rampPlusB_ModeStart,
    nullptr
};

static Form formRampPlusB(TypeForm::RampPlus, params_RampPlusB, &waves[Chan::B]);


static PFrequency  rampMinusB_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParamAmplitude  rampMinusB_Amplitude;
static ParamOffset     rampMinusB_Offset;
static PModeStart  rampMinusB_ModeStart(Param::EFuncActive, namesModeStartFree);

static Param *params_RampMinusB[] =
{
    &rampMinusB_Frequency,
    &rampMinusB_Amplitude,
    &rampMinusB_Offset,
    &rampMinusB_ModeStart,
    nullptr
};

static Form formRampMinusB(TypeForm::RampMinus, params_RampMinusB, &waves[Chan::B]);


static PFrequency  triangleB_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParamAmplitude  triangleB_Amplitude;
static ParamOffset     triangleB_Offset;
static PModeStart  triangleB_ModeStart(Param::EFuncActive, namesModeStartFree);

static Param *params_TriangleB[] =
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

static PFrequency  freeB_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParamAmplitude  freeB_Amplitude;
static ParamOffset     freeB_Offset;
static PModeStart  freeB_ModeStart(Param::EFuncActive, namesModeStartFree);
static BParam     freeB_Choose("Выбрать", "Choose", OnChoose_FileB);

static Param *params_FreeB[] =
{
    &freeB_Frequency,
    &freeB_Amplitude,
    &freeB_Offset,
    &freeB_ModeStart,
    &freeB_Choose,
    nullptr
};

static Form formFreeB(TypeForm::Free, params_FreeB, &waves[Chan::B]);


static PFrequency meanderB_Frequency(FREQUENCY_DDS_MIN, FREQUENCY_DDS_MAX);
static ParamAmplitude meanderB_Amplitude;
static ParamOffset    meanderB_Offset;

static Param *params_MeanderB[] =
{
    &meanderB_Frequency,
    &meanderB_Amplitude,
    &meanderB_Offset,
    nullptr
};

static Form formMeanderB(TypeForm::Meander, params_MeanderB, &waves[Chan::B]);


static bool FuncActive_PeriodImpulseB()
{
    if (FORM_A == A::Impulse::form)
    {
        if (A::Impulse::start_stop->GetChoice() == 1)
        {
            return false;
        }
    }

    return (impulseB_ModeStart.GetChoice() == 0);
}

static bool FuncActive_ModeStartImpulseB()
{
    if (FORM_A == A::Impulse::form)
    {
        if (A::Impulse::start_stop->GetChoice() == 1)
        {
            return false;
        }
    }

    return true;
}


static PPeriod    impulseB_Period   (FuncActive_PeriodImpulseB, Value("100", Order::Kilo), Value("100", Order::Micro));
static PDuration  impulseB_Duration (Value("100", Order::Kilo), Value("20", Order::Micro));
static ParamAmplitude impulseB_Amplitude;
static ParamOffset    impulseB_Offset;
static PPolarity  impulseB_Polarity(namesPolarity);
static PModeStart impulseB_ModeStart(FuncActive_ModeStartImpulseB, namesModeStartImpulse);

PDuration  *B::Impulse::duration = &impulseB_Duration;

static Param *params_ImpulseB[] =
{
    &impulseB_Period,
    &impulseB_Duration,
    &impulseB_Amplitude,
    &impulseB_Offset,
    &impulseB_Polarity,
    &impulseB_ModeStart,
    &impulse_Clock,
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
    Wave(ChA, formsA),
    Wave(ChB, formsB)
};


Form                   *A::Impulse::form = &formImpulseA;

Form                   *A::Packet::form = &formPacketImpulseA;
PPacketPeriod  *A::Packet::period_packet = &packetA_PacketPeriod;

Form                   *B::Impulse::form = &formImpulseB;
PPeriod        *B::Impulse::period = &impulseB_Period;
PModeStart     *B::Impulse::mode_start = &impulseB_ModeStart;
