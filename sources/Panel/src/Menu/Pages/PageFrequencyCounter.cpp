#include "PageFrequencyCounter.h"
#include "Settings/Settings.h"
#include "Generator/Generator.h"
#include "FrequencyMeter/FrequencyMeter.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pFrequencyCounter;
Page *PageFrequencyCounter::pointer = (Page *)&pFrequencyCounter;

static void OnPress_Measure(bool);
static void OnPress_Interval(bool);
static void OnPress_BillingTime(bool);
static void OnPress_Resist(bool);
static void OnPress_Couple(bool);
static void OnPress_Filtr(bool);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cEnableFrequencyCounter,                                                                               //--- ЧАСТОТОМЕР - ОТОБРАЖЕНИЕ ---
    "ОТОБРАЖЕНИЕ", "DISPLAY",
    "Включает и выключает вывод частотомера.",
    "Turns the frequency meter output on and off.",
    DISABLED_RU, DISABLED_EN, "Отображение показаний частотомера отключено.",
                              "Display of frequency meter readings is disabled.",
    ENABLED_RU,  ENABLED_EN,  "В нижней части экрана выводятся показания частотомера.",
                              "The bottom of the screen displays the frequency meter.",
    FLAG_1, BIT_FREQ_ENABLED, pFrequencyCounter, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cMeasure,                                                                                                //--- ЧАСТОТОМЕР - ИЗМЕРЕНИЕ ---
    "ИЗМЕРЕНИЕ", "MEASURE",
    "Установка режима работы",
    "Setting the operating mode",
    "Частота", "Frequency", "Измерение частоты",
                            "Frequency measurement",
    "Период", "Period",     "Измерение периода",
                            "Period measurement",
    FLAG_1, BIT_FREQ_MEASURE, pFrequencyCounter, FuncActive, OnPress_Measure, FuncDraw
)

static void OnPress_Measure(bool)
{
    PageFrequencyCounter::WriteRegisterRG9();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cInterval,                                                                                        //--- ЧАСТОТОМЕР - ИНТЕРВАЛ ЗАПУСКА ---
    "ИНТЕРВАЛ ЗАПУСКА", "START INTERVAL",
    "Выбор интервала запуска измерений частоты.",
    "Selecting the frequency measurement start interval.",
    "1 c", "1 s",   "Запуск процесса измерения частомера производится с интервалом 1 секунда.",
                    "The measurement of the frequency meter is started at intervals of 1 second.",
    "10 с", "10 s", "Запуск процесса измерения частомера производится с интервалом 10 секунда.",
                    "The process of measuring the frequency meter is started at intervals of 10 seconds.",
    FLAG_1, BIT_FREQ_INTERVAL, pFrequencyCounter, FuncActive, OnPress_Interval, FuncDraw
)

static void OnPress_Interval(bool)
{
    PageFrequencyCounter::WriteRegisterRG9();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_5(cBillingTime,                                                                                          //--- ЧАСТОТОМЕР - ВРЕМЯ СЧЁТА ---
    "ВРЕМЯ СЧЁТА", "BILLING TIME",
    "Выбор времени проведения измерения.",
    "Selecting the measurement time.",
    "1 мс",     "1 ms",     "Длительность измерения 1 миллисекунда.",
                            "The measurement duration is 1 millisecond.",
    "10 мс",    "10 ms",    "Длительность измерения 10 миллисекунд.",
                            "The measurement duration is 10 milliseconds.",
    "100 мс",   "100 ms",   "Длительность измерения 100 миллисекунд.",
                            "Duration of measurement 100 milliseconds.",
    "1000 мс",  "1000 ms",  "Длительность измерения 1000 миллисекунд.",
                            "The measurement duration is 1000 milliseconds.",
    "10000 мс", "10000 ms", "Длительность измерения 10000 миллисекунд.",
                            "The measurement duration is 10,000 milliseconds.",
    FREQ_BILLING_TIME, pFrequencyCounter, FuncActive, OnPress_BillingTime, FuncDraw
)

static void OnPress_BillingTime(bool)
{
    PageFrequencyCounter::WriteRegisterRG9();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cResist,
             "СОПРОТИВЛЕНИЕ ВХОДА", "INPUT RESISTANCE",
             "Управление сопротивлением входа частотомера",
             "Control of the resistance of the frequency meter input",
             "1 МОм", "1 MOhm", "Сопротивление входа 1 МОм",
             "Input Impedance 1 MOhm",
             "50 Ом", "50 Ohm", "Сопротивление входа 50 Ом",
             "Input Impedance 50 Ohm",
             FLAG_1, BIT_FREQ_RESIST, pFrequencyCounter, FuncActive, OnPress_Resist, FuncDraw
)

static void OnPress_Resist(bool)
{
    Generator::LoadRegister(Register::FreqMeter_Resist, (uint)FREQ_RESIST);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cCouple,
             "ВХОД", "COUPLE",
             "Пропускает/запрещает постоянную составляющую",
             "Skips / prohibits the DC component",
             "Перем", "AC", "Постоянная составляющая поступает на вход частотомера",
             "The DC component is fed to the frequency meter input",
             "Пост", "DC", "Постоянная составляющая не поступает на вход частотомера",
             "The DC component is not input to the frequency meter input",
             FLAG_1, BIT_FREQ_COUPLE, pFrequencyCounter, FuncActive, OnPress_Couple, FuncDraw
)

static void OnPress_Couple(bool)
{
    Generator::LoadRegister(Register::FreqMeter_Couple, (uint)FREQ_COUPLE);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cFiltr,
             "ФНЧ", "LPF",
             "Включает/отключает фильтр нижних частот на входе частотомера",
             "Enables / disables the low-pass filter at the input of the frequency meter",
             DISABLED_RU, DISABLED_EN, "ФНЧ на входе частотомера отключен",
             "LPF at the frequency meter input is disabled",
             ENABLED_RU, ENABLED_EN, "ФНЧ на входе частотомера водключен",
             "LPF at the input of the frequency meter is turned on",
             FLAG_1, BIT_FREQ_FILTR, pFrequencyCounter, FuncActive, OnPress_Filtr, FuncDraw
)

static void OnPress_Filtr(bool)
{
    Generator::LoadRegister(Register::FreqMeter_Filtr, (uint)FREQ_FILTR);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_7(pFrequencyCounter,                                                                                                     //--- ЧАСТОТОМЕР ---
    "ЧАСТОТОМЕР", "FREQUENCY METER",
    "Управление фукнциями частотомера.",
    "Control of frequency meter functions.",
    cEnableFrequencyCounter,
    cMeasure,
    cInterval,
    cBillingTime,
    cResist,
    cCouple,
    cFiltr,
    Page::FrequencyCounter, 0, FuncActive, FuncPress, FuncOnKey
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageFrequencyCounter::WriteRegisterRG9()
{
    uint data = 0;

    //----------- Режим работы ------------------

    static const uint maskMeasure[2] = {0, 1};
    data |= maskMeasure[FREQ_MEASURE];

    //----------- Число усредняемых периодов ----

    static const uint maskAvePeriod[5] =
    {
        BINARY_U8(00000000),    // 1
        BINARY_U8(00000010),    // 10
        BINARY_U8(00000110),    // 100
        BINARY_U8(00001010),    // 1000
        BINARY_U8(00001110)     // 10000
    };
    data |= maskAvePeriod[FREQ_AVE_PERIOD];

    //---------- Время индикации ----------------

    static const uint maskInterval[2] =
    {
        BINARY_U8(00000000),
        BINARY_U8(00010000)
    };
    data |= maskInterval[FREQ_INTERVAL];

    //--------- Время счёта ---------------------

    static const uint maskTime[5] =
    {
        BINARY_U8(00000000),    // 1 мс
        BINARY_U8(00100000),    // 10 мс
        BINARY_U8(01000000),    // 100 мс
        BINARY_U8(01100000),    // 1 с
        BINARY_U8(10000000)     // 10 с
    };
    data |= maskTime[FREQ_BILLING_TIME];

    //--------- Метки времени -------------------

    static const uint maskTimeStamp[5] =
    {
        BINARY_U8(00001010),    // 1 кГц
        BINARY_U8(00000110),    // 10 кГц
        BINARY_U8(00000010),    // 100 кГц
        BINARY_U8(00000001),    // 1 МГц
        BINARY_U8(00000000)     // 10 МГц
    };

    data |= (maskTimeStamp[FREQ_TIME_STAMPS] << 8);

    if(FREQ_TEST)
    {
        _SET_BIT(data, 12);
    }

    Generator::LoadRegister(Register::FPGA_RG9_FreqMeter, data);
    FrequencyMeter::SetInactive();
}
