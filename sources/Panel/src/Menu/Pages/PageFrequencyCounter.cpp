#include "stdafx.h"
#ifndef WIN32
#include "PageFrequencyCounter.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Log.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pFrequencyCounter;
Page *PageFrequencyCounter::pointer = (Page *)&pFrequencyCounter;


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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Measure(bool)
{
    Tune_Page();
    PageFrequencyCounter::WriteRegisterRG9();
}

DEF_CHOICE_3( cMeasure,                                                                                                                                      //--- ЧАСТОТОМЕР - Измерение ---
    "Измерение",
    "Установка режима работы"
    ,
    "Отключено", "Измерения отключены",
    "Частота",   "Измерение частоты",
    "Период",    "Измерение периода",
    FREQ_METER_MEASURE, pFrequencyCounter, Item::FuncActive, OnPress_Measure, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cInterval,                                                                                                                              //--- ЧАСТОТОМЕР - Интервал запуска ---
    "Интервал запуска",
    "Выбор интервала запуска измерений частоты."
    ,
    "1 c", "Запуск процесса измерения частомера производится с интервалом 1 секунда.",
    "10 с", "Запуск процесса измерения частомера производится с интервалом 10 секунда.",
    FLAG_1, BIT_FREQ_INTERVAL, pFrequencyCounter, Item::FuncActive, OnPress_Interval, FuncDraw
)

static void OnPress_Interval(bool)
{
    PageFrequencyCounter::WriteRegisterRG9();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_5( cBillingTime,                                                                                                                                //--- ЧАСТОТОМЕР - Время счёта ---
    "Время счёта",
    "Выбор времени проведения измерения."
    ,
    "1 мс",     "Длительность измерения 1 миллисекунда.",
    "10 мс",    "Длительность измерения 10 миллисекунд.",
    "100 мс",   "Длительность измерения 100 миллисекунд.",
    "1000 мс",  "Длительность измерения 1000 миллисекунд.",
    "10000 мс", "Длительность измерения 10000 миллисекунд.",
    FREQ_BILLING_TIME, pFrequencyCounter, Item::FuncActive, OnPress_BillingTime, FuncDraw
)

static void OnPress_BillingTime(bool)
{
    PageFrequencyCounter::WriteRegisterRG9();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cResist,                                                                                                                              //--- ЧАСТОТОМЕР - Сопротивление входа ---
    "Сопротивление входа",
    "Управление сопротивлением входа частотомера"
    ,
    "1 МОм", "Сопротивление входа 1 МОм",
    "50 Ом", "Сопротивление входа 50 Ом",
    FLAG_1, BIT_FREQ_RESIST, pFrequencyCounter, Item::FuncActive, OnPress_Resist, FuncDraw
)

static void OnPress_Resist(bool)
{
    Generator::LoadRegister(Register::FreqMeter_Resist, (uint)FREQ_RESIST);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cCouple,                                                                                                                                             //--- ЧАСТОТОМЕР - Вход ---
    "Вход",
    "Пропускает/запрещает постоянную составляющую"
    ,
    "Перем","Постоянная составляющая поступает на вход частотомера",
    "Пост", "Постоянная составляющая не поступает на вход частотомера",
    FLAG_1, BIT_FREQ_COUPLE, pFrequencyCounter, Item::FuncActive, OnPress_Couple, FuncDraw
)

static void OnPress_Couple(bool)
{
    Generator::LoadRegister(Register::FreqMeter_Couple, (uint)FREQ_COUPLE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cFiltr,                                                                                                                                               //--- ЧАСТОТОМЕР - ФНЧ ---
    "ФНЧ",
    "Включает/отключает фильтр нижних частот на входе частотомера"
    ,
    DISABLED_RU, "ФНЧ на входе частотомера отключен",
    ENABLED_RU,  "ФНЧ на входе частотомера водключен",
    FLAG_1, BIT_FREQ_FILTR, pFrequencyCounter, Item::FuncActive, OnPress_Filtr, FuncDraw
)

static void OnPress_Filtr(bool)
{
    Generator::LoadRegister(Register::FreqMeter_Filtr, (uint)FREQ_FILTR);
}

DEF_CHOICE_5(cAvePeriod,                                                                                                                                //--- ЧАСТОТОМЕР - ЧИСЛО ПЕРИОДОВ ---
    "Число периодов",
    "Выбор числа усредняемых периодов в режиме измерения периода"
    ,
    "1",     "Измерения производить по одному периоду",
    "10",    "Измерения производить по десяти периодам",
    "100",   "Измерения производить по ста периодам",
    "1000",  "Измерения производить по тысяче периодов",
    "10000", "Измерения производить по десяти тысячам периодов",
    FREQ_AVE_PERIOD, pFrequencyCounter, Item::FuncActive, OnPress_AvePeriod, FuncDraw
)

static void OnPress_AvePeriod(bool)
{
    PageFrequencyCounter::WriteRegisterRG9();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_5(cTimeStamps,                                                                                                                                //--- ЧАСТОТОМЕР - Метки времени ---
    "Метки времени",
    ""
    ,
    "1 кГц",    "", 
    "10 кГц",   "", 
    "100 кГц",  "", 
    "1 МГц",    "", 
    "10 МГц",   "", 
    FREQ_TIME_STAMPS, pFrequencyCounter, Item::FuncActive, OnPress_TimeStamps, FuncDraw
)

static void OnPress_TimeStamps(bool)
{
    PageFrequencyCounter::WriteRegisterRG9();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cTest,                                                                                                                                               //--- ЧАСТОТОМЕР - Тест ---
    "Тест",
    "Включение/отключение тестового режима"
    ,
    DISABLED_RU, "",
    ENABLED_RU,  "",
    FLAG_1, BIT_FREQ_TEST, pFrequencyCounter, Item::FuncActive, OnPress_Test, FuncDraw
)

static void OnPress_Test(bool)
{
    PageFrequencyCounter::WriteRegisterRG9();
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_FreqLevel()
{
    FrequencyMeter::LoadLevel();
}

DEF_GOVERNOR( gLevel,                                                                                                                                          //--- ЧАСТОТОМЕР - Уровень ---
    "Уровень",
    "Подстройка уровня синхронизации",
    FREQ_LEVEL, -100, 100, pFrequencyCounter, Item::FuncActive, OnChange_FreqLevel, EmptyFuncVV, 0
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_Hysteresis()
{
    FrequencyMeter::LoadHysteresis();
}

DEF_GOVERNOR( gHysteresis,                                                                                                                                  //--- ЧАСТОТОМЕР - Гистерезис ---
    "Гистерезис",
    "Задаёт гистерезис для уменьшения влияния помех на точность измерений",
    FREQ_HYSTERESIS, 0, 100, pFrequencyCounter, Item::FuncActive, OnChange_Hysteresis, EmptyFuncVV, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_8( pFrequencyCounter,  // -V641 // -V1027                                                                                                                       //--- ЧАСТОТОМЕР ---
    "ЧАСТОТОМЕР",
    "Управление фукнциями частотомера."
    ,
    &cMeasure,       ///< ЧАСТОТОМЕР - Измерение
    &gLevel,         ///< ЧАСТОТОМЕР - Уровень
    0,
    0,
    &cResist,        ///< ЧАСТОТОМЕР - Сопротивление
    &cCouple,        ///< ЧАСТОТОМЕР - Вход
    &cFiltr,         ///< ЧАСТОТОМЕР - ФНЧ
    &cTest,          ///< ЧАСТОТОМЕР - Тест
    Page::FrequencyCounter, Menu::mainPage, Item::FuncActive, FuncPress, FuncOnKey, Page::FuncDraw
)


static void Tune_Page()
{
    PageBase *page = (PageBase *)&pFrequencyCounter;

    if (FREQ_METER_MEASURE_IS_FREQ)
    {
        page->items[2] = (Item *)&cBillingTime; // -V641
        page->items[3] = 0;
    }
    else if (FREQ_METER_MEASURE_IS_PERIOD)
    {
        page->items[2] = (Item *)&cTimeStamps;  // -V641
        page->items[3] = (Item *)&cAvePeriod;   // -V641
    }
    else
    {
        page->items[2] = 0;
        page->items[3] = 0;
    }
}

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)       // cast truncates constant value
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageFrequencyCounter::WriteRegisterRG9()
{
    uint data = 0;

    //----------- Режим работы ------------------
    if(FREQ_METER_MEASURE_IS_PERIOD)
    {
        data |= 1;
    }

    //----------- Число усредняемых периодов ----

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
    data |= maskInterval[FREQ_INTERVAL];

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

    Generator::LoadRegister(Register::FPGA_RG9_FreqMeter, data);
    FrequencyMeter::SetInactive();
}

#ifdef WIN32
#pragma warning(pop)
#endif
