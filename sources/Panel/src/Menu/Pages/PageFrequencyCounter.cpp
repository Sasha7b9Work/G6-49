#include "PageFrequencyCounter.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pFrequencyCounter;
Page *PageFrequencyCounter::pointer = (Page *)&pFrequencyCounter;


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
    FLAG_3, BIT_FREQ_MEASURE, pFrequencyCounter, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cInterval,                                                                                        //--- ЧАСТОТОМЕР - ИНТЕРВАЛ ЗАПУСКА ---
    "ИНТЕРВАЛ ЗАПУСКА", "START INTERVAL",
    "Выбор интервала запуска измерений частоты.",
    "Selecting the frequency measurement start interval.",
    "1 c", "1 s",   "Запуск процесса измерения частомера производится с интервалом 1 секунда.",
                    "The measurement of the frequency meter is started at intervals of 1 second.",
    "10 с", "10 s", "Запуск процесса измерения частомера производится с интервалом 10 секунда.",
                    "The process of measuring the frequency meter is started at intervals of 10 seconds.",
    FLAG_1, BIT_FREQ_INTERVAL, pFrequencyCounter, FuncActive, FuncChangedChoice, FuncDraw
)

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
    BILLING_TIME, pFrequencyCounter, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_4(pFrequencyCounter,                                                                                                     //--- ЧАСТОТОМЕР ---
    "ЧАСТОТОМЕР", "FREQUENCY METER",
    "Управление фукнциями частотомера.",
    "Control of frequency meter functions.",
    cEnableFrequencyCounter,
    cMeasure,
    cInterval,
    cBillingTime,
    Page_FrequencyCounter, 0, FuncActive, FuncPress, FuncOnKey
)
