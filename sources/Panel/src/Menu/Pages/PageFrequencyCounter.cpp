#include "PageFrequencyCounter.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pFrequencyCounter;
Page *PageFrequencyCounter::pointer = (Page *)&pFrequencyCounter;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cEnableFrequencyCounter,                                                                               //--- ЧАСТОТОМЕР - ОТОБРАЖЕНИЕ ---
    "ОТОБРАЖЕНИЕ", "DISPLAY",
    "Включает и выключает вывод частотомера",
    "Turns the frequency meter output on and off",
    DISABLED_RU, DISABLED_EN, "",
                              "",
    ENABLED_RU,  ENABLED_EN,  "",
                              "",
    FREQ_COUNTER_ENABLED, pFrequencyCounter, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cInterval,                                                                                        //--- ЧАСТОТОМЕР - ИНТЕРВАЛ ЗАПУСКА ---
    "ИНТЕРВАЛ ЗАПУСКА", "START INTERVAL",
    "Выбор интервала запуска измерений частоты",
    "Selecting the frequency measurement start interval",
    "1 c", "1 s",   "",
                    "",
    "10 с", "10 s", "",
                    "",
    INTERVAL, pFrequencyCounter, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_5(cBillingTime,
    "ВРЕМЯ СЧЁТА", "BILLING TIME",
    "Выбор времени проведения измерения",
    "Selecting the measurement time",
    "1 мс",     "1 ms",     "",
                            "",
    "10 мс",    "10 ms",    "",
                            "",
    "100 мс",   "100 ms",   "",
                            "",
    "1000 мс",  "1000 ms",  "",
                            "",
    "10000 мс", "10000 ms", "",
                            "",
    BILLING_TIME, pFrequencyCounter, FuncActive, FuncChangedChoice, FuncDraw
)


//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3(pFrequencyCounter,
    "ЧАСТОТОМЕР", "FRUENCY METER",
    "Управление фукнциями частотомера",
    "Control of frequency meter functions",
    cEnableFrequencyCounter,
    cInterval,
    cBillingTime,
    Page_FrequencyCounter, 0, FuncActive, FuncPress
)
