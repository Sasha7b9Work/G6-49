#include "PageFrequencyCounter3.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pFrequencyCounte3;
Page *PageFrequencyCounter3::pointer = (Page *)&pFrequencyCounte3;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_5(cAvePeriod,                                                                                       //--- ЧАСТОТОМЕР 3 - ЧИСЛО ПЕРИОДОВ ---
    "ЧИСЛО ПЕРИОДОВ", "NUMBER OF PERIODS",
    "Выбор числа усредняемых периодов в режиме измерения периода",
    "Selecting the number of averaging periods in the period measurement mode",
    "1", "1",           "Измерения производить по одному периоду",
                        "Measurements should be made for one period",
    "10", "10",         "Измерения производить по десяти периодам",
                        "Measurements are made for ten periods",
    "100", "100",       "Измерения производить по ста периодам",
                        "Measurements are made for a hundred periods",
    "1000", "1000",     "Измерения производить по тысяче периодов",
                        "Measure to produce for a thousand periods",
    "10000", "10000",   "Измерения производить по десяти тысячам периодов",
                        "Measurements are made for ten thousand periods",
    FREQ_AVE_PERIOD, pFrequencyCounte3, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_5(cTimeStamps,                                                                                       //--- ЧАСТОТОМЕР 3 - МЕТКИ ВРЕМЕНИ ---
    "МЕТКИ ВРЕМЕНИ", "TIME STAMPS",
    "",
    "",
    "1 кГц",    "1 kHz",    "", "",
    "10 кГц",   "10 kHz",   "", "",
    "100 кГц",  "100 kHz",  "", "",
    "1 МГц",    "1 MHz",    "", "",
    "10 МГц",   "10 MHz",   "", "",
    FREQ_TIME_STAMPS, pFrequencyCounte3, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_2(pFrequencyCounte3,                                                                                                   //--- ЧАСТОТОМЕР 3 ---
    "ЧАСТОТОМЕР 3", "FREQUENCY COUNTER 3",
    "Управление функциями частотомера",
    "Control of frequency meter functions",
    cAvePeriod,
    cTimeStamps,
    Page_FrequencyCounter3, 0, FuncActive, FuncPress
)
