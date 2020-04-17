#include "PageFrequencyCounter.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Log.h"


extern const PageBase pFrequencyCounter;
Page *PageFrequencyCounter::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pFrequencyCounter));


/// Настроить вид страницы в соответствии с режимом измерения
static void Tune_Page();
static void OnPress_Interval(bool);
static void OnPress_BillingTime(bool);
static void OnPress_Resist(bool);
static void OnPress_Couple(bool);
static void OnPress_Filtr(bool);
static void OnPress_Test(bool);
static void OnPress_TimeStamps(bool);
static void OnPress_AvePeriod(bool);


static void OnPress_Measure(bool)
{
    Tune_Page();
    PageFrequencyCounter::WriteRegisterRG9();
}

DEF_CHOICE_3( cMeasure,                                                                                                                                      //--- ЧАСТОТОМЕР - Измерение ---
    "Измерение", "Measurement",
    "Установка режима работы", "Setting operation mode",
    "Отключено", "Off",       "Измерения отключены", "Measurements off",
    "Частота",   "Frequency", "Измерение частоты",   "Frequency measurement",
    "Период",    "Period",    "Измерение периода",   "Period measurement",
    FREQ_METER_MEASURE, pFrequencyCounter, Item::FuncActive, OnPress_Measure, FuncDraw
)


DEF_CHOICE_2( cInterval,                                                                                                                              //--- ЧАСТОТОМЕР - Интервал запуска ---
    "Интервал запуска", "Launch interval",
    "Выбор интервала запуска измерений частоты", "Frequency start interval selection",
    "1 c",  "1 s",  "Запуск процесса измерения частомера производится с интервалом 1 секунда",  "The process of measuring the frequency meter is started at intervals of 1 second",
    "10 с", "10 s", "Запуск процесса измерения частомера производится с интервалом 10 секунда", "The measurement process starts with a 10 second interval",
    set.freq_interval, pFrequencyCounter, Item::FuncActive, OnPress_Interval, FuncDraw
)

volatile const ChoiceBase *pcInterval = &cInterval;

static void OnPress_Interval(bool)
{
    PageFrequencyCounter::WriteRegisterRG9();
}


DEF_CHOICE_5( cBillingTime,                                                                                                                                //--- ЧАСТОТОМЕР - Время счёта ---
    "Время счёта", "Counting time",
    "Выбор времени проведения измерения", "Timing measurement",
    "1 мс",     "1 ms",     "Длительность измерения 1 миллисекунда",    "1 millisecond measurement duration",
    "10 мс",    "10 ms",    "Длительность измерения 10 миллисекунд",    "Measurement duration 10 milliseconds",
    "100 мс",   "100 ms",   "Длительность измерения 100 миллисекунд",   "Measurement duration 100 milliseconds",
    "1000 мс",  "1000 ms",  "Длительность измерения 1000 миллисекунд",  "Measurement duration 1000 milliseconds",
    "10000 мс", "10000 ms", "Длительность измерения 10000 миллисекунд", "Measurement duration 10000 milliseconds",
    FREQ_BILLING_TIME, pFrequencyCounter, Item::FuncActive, OnPress_BillingTime, FuncDraw
)

static void OnPress_BillingTime(bool)
{
    PageFrequencyCounter::WriteRegisterRG9();
}


DEF_CHOICE_2(cResist,                                                                                                                              //--- ЧАСТОТОМЕР - Сопротивление входа ---
    "Сопротивление входа", "Input impedance",
    "Управление сопротивлением входа частотомера", "Frequency counter input resistance control",
    "1 МОм", "1 MOhm", "Сопротивление входа 1 МОм", "Input impedance 1 MOhm",
    "50 Ом", "50 Ohm", "Сопротивление входа 50 Ом", "50 ohm input impedance",
    set.freq_resist, pFrequencyCounter, Item::FuncActive, OnPress_Resist, FuncDraw
)

static void OnPress_Resist(bool)
{
    PGenerator::LoadRegister(Register::FreqMeter_Resist, (uint)set.freq_resist);
}


DEF_CHOICE_2(cCouple,                                                                                                                                             //--- ЧАСТОТОМЕР - Вход ---
    "Вход", "Couple",
    "Пропускает/запрещает постоянную составляющую", "Skips / Disables DC",
    "Перем", "Alternate", "Постоянная составляющая поступает на вход частотомера",    "The constant component is fed to the input of the frequency counter",
    "Пост",  "Direct",    "Постоянная составляющая не поступает на вход частотомера", "The constant component does not go to the input of the frequency meter",
    set.freq_couple, pFrequencyCounter, Item::FuncActive, OnPress_Couple, FuncDraw
)

static void OnPress_Couple(bool)
{
    PGenerator::LoadRegister(Register::FreqMeter_Couple, (uint)set.freq_couple);
}


DEF_CHOICE_2_OLD(cFiltr,                                                                                                                                               //--- ЧАСТОТОМЕР - ФНЧ ---
    "ФНЧ", "LPF",
    "Включает/отключает фильтр нижних частот на входе частотомера", "Enables / disables the low-pass filter at the input of the frequency meter",
    DISABLED_RU, DISABLED_EN, "ФНЧ на входе частотомера отключен",  "LPF at the input of the frequency meter is disabled",
    ENABLED_RU,  ENABLED_EN,  "ФНЧ на входе частотомера водключен", "Low-pass filter at the input of the frequency meter",
    FLAG, BIT_FREQ_FILTR, pFrequencyCounter, Item::FuncActive, OnPress_Filtr, FuncDraw
)

static void OnPress_Filtr(bool)
{
    PGenerator::LoadRegister(Register::FreqMeter_Filtr, (uint)FREQ_FILTR);
}

DEF_CHOICE_5(cAvePeriod,                                                                                                                                //--- ЧАСТОТОМЕР - ЧИСЛО ПЕРИОДОВ ---
    "Число периодов", "Number of periods",
    "Выбор числа усредняемых периодов в режиме измерения периода", "Choosing the number of averaged periods in period measurement mode",
    "1",     "1",     "Измерения производить по одному периоду",          "Measurements on one period",
    "10",    "10",    "Измерения производить по десяти периодам",         "Measure over ten periods",
    "100",   "100",   "Измерения производить по ста периодам",            "Measurements over a hundred periods",
    "1000",  "1000",  "Измерения производить по тысяче периодов",         "Measure over a thousand periods",
    "10000", "10000", "Измерения производить по десяти тысячам периодов", "Measure over ten thousand periods",
    FREQ_AVE_PERIOD, pFrequencyCounter, Item::FuncActive, OnPress_AvePeriod, FuncDraw
)

static void OnPress_AvePeriod(bool)
{
    PageFrequencyCounter::WriteRegisterRG9();
}


DEF_CHOICE_5(cTimeStamps,                                                                                                                                //--- ЧАСТОТОМЕР - Метки времени ---
    "Метки времени", "Time stamps",
    "", "",
    "1 кГц",   "1 kHz",   "", "",
    "10 кГц",  "10 kHz",  "", "",
    "100 кГц", "100 kHz", "", "",
    "1 МГц",   "1 MHz",   "", "",
    "10 МГц",  "10 MHz",  "", "",
    FREQ_TIME_STAMPS, pFrequencyCounter, Item::FuncActive, OnPress_TimeStamps, FuncDraw
)

static void OnPress_TimeStamps(bool)
{
    PageFrequencyCounter::WriteRegisterRG9();
}


DEF_CHOICE_2_OLD(cTest,                                                                                                                                               //--- ЧАСТОТОМЕР - Тест ---
    "Тест", "Test",
    "Включение/отключение тестового режима", "Enable / disable test mode",
    DISABLED_RU, DISABLED_EN, "", "",
    ENABLED_RU,  ENABLED_EN,  "", "",
    FLAG, BIT_FREQ_TEST, pFrequencyCounter, Item::FuncActive, OnPress_Test, FuncDraw
)

static void OnPress_Test(bool)
{
    PageFrequencyCounter::WriteRegisterRG9();
}



static void OnChange_FreqLevel()
{
    PFreqMeter::LoadLevel();
}

DEF_GOVERNOR( gLevel,                                                                                                                                          //--- ЧАСТОТОМЕР - Уровень ---
    "Уровень", "Level",
    "Подстройка уровня синхронизации", "Sync level adjustment",
    FREQ_LEVEL, -100, 100, pFrequencyCounter, Item::FuncActive, OnChange_FreqLevel, EmptyFuncVV, 0
)



static void OnChange_Hysteresis()
{
    PFreqMeter::LoadHysteresis();
}

DEF_GOVERNOR( gHysteresis,                                                                                                                                  //--- ЧАСТОТОМЕР - Гистерезис ---
    "Гистерезис", "Hysteresis",
    "Задаёт гистерезис для уменьшения влияния помех на точность измерений", "Sets hysteresis to reduce the effect of interference on measurement accuracy",
    FREQ_HYSTERESIS, 0, 100, pFrequencyCounter, Item::FuncActive, OnChange_Hysteresis, EmptyFuncVV, 0
)

volatile const GovernorBase *pgHysteresis = &gHysteresis;


DEF_PAGE_8( pFrequencyCounter,                                                                                                                                           //--- ЧАСТОТОМЕР ---
    "ЧАСТОТОМЕР", "FREQMETER", //-V641
    "Управление фукнциями частотомера", "Freqmeter control",
    &cMeasure,       ///< ЧАСТОТОМЕР - Измерение
    &gLevel,         ///< ЧАСТОТОМЕР - Уровень
    Item::EmptyLight(),
    Item::EmptyLight(),
    &cResist,        ///< ЧАСТОТОМЕР - Сопротивление
    &cCouple,        ///< ЧАСТОТОМЕР - Вход
    &cFiltr,         ///< ЧАСТОТОМЕР - ФНЧ
    &cTest,          ///< ЧАСТОТОМЕР - Тест
    Page::FrequencyCounter, Menu::mainPage, Item::FuncActive, Page::FuncEnter, FuncOnKey, Page::FuncDraw
)


static void Tune_Page()
{
    PageBase *page = const_cast<PageBase *>(&pFrequencyCounter);

    if (FREQ_METER_MEASURE_IS_FREQ)
    {
        page->items[2] = reinterpret_cast<Item *>(const_cast<ChoiceBase *>(&cBillingTime));
        page->items[3] = Item::EmptyLight();
    }
    else if (FREQ_METER_MEASURE_IS_PERIOD)
    {
        page->items[2] = reinterpret_cast<Item *>(const_cast<ChoiceBase *>(&cTimeStamps));
        page->items[3] = reinterpret_cast<Item *>(const_cast<ChoiceBase *>(&cAvePeriod));
    }
    else
    {
        page->items[2] = Item::EmptyLight();
        page->items[3] = Item::EmptyLight(); //-V656
    } //-V656
}

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)       // cast truncates constant value
#endif


void PageFrequencyCounter::WriteRegisterRG9()
{
    uint data = 0;

     //--- Режим работы ------------------
    if(FREQ_METER_MEASURE_IS_PERIOD)
    {
        data |= 1;
    }

    //--- Число усредняемых периодов ----

    static const uint maskAvePeriod[5] =
    {
        BINARY_U8(00000000),    // -V2501   // 1
        BINARY_U8(00000010),    // -V2501   // 10
        BINARY_U8(00000110),    // -V2501   // 100
        BINARY_U8(00001010),    // -V2501   // 1000
        BINARY_U8(00001110)     // -V2501   // 10000
    };
    data |= maskAvePeriod[FREQ_AVE_PERIOD];

    //---------- Время индикации ----------------

    static const uint maskInterval[2] =
    {
        BINARY_U8(00000000),    // -V2501
        BINARY_U8(00010000)     // -V2501
    };
    data |= maskInterval[set.freq_interval];

    //--------- Время счёта ---------------------

    static const uint maskTime[5] =
    {
        BINARY_U8(00000000),    // -V2501               // 1 мс
        BINARY_U8(00100000),    // -V2501               // 10 мс
        BINARY_U8(01000000),    // -V2501   // -V536    // 100 мс
        BINARY_U8(01100000),    // -V2501               // 1 с
        BINARY_U8(10000000)     // -V2501               // 10 с
    };
    data |= maskTime[FREQ_BILLING_TIME];

    //--------- Метки времени -------------------

    static const uint maskTimeStamp[5] =
    {
        BINARY_U8(00001010),    // -V2501    // 1 кГц
        BINARY_U8(00000110),    // -V2501    // 10 кГц
        BINARY_U8(00000010),    // -V2501    // 100 кГц
        BINARY_U8(00000001),    // -V2501    // 1 МГц
        BINARY_U8(00000000)     // -V2501    // 10 МГц
    };

    data |= (maskTimeStamp[FREQ_TIME_STAMPS] << 8);

    if(FREQ_TEST)
    {
        _SET_BIT(data, 12);
    }

    PGenerator::LoadRegister(Register::FPGA_RG9_FreqMeter, data);
    PFreqMeter::SetInactive();
}

#ifdef WIN32
#pragma warning(pop)
#endif
