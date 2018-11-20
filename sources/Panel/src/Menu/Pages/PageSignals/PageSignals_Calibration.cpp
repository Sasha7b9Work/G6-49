#include "stdafx.h"
#ifndef WIN32
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageSignals.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageCalibration;
Page *PageSignals::PageCalibration::pointer = (Page *)&pageCalibration;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChange_NegativeAD9952()
{

}

DEF_GOVERNOR( gNegativeAD9952,                                                                                                         //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА - AD9952 -5В ---
    "AD9952 -5В", "AD9952 -5V",
    "Настройка уровня -5В AD9952",
    "Level setting -5V AD9952",
    CAL_AD9952_OFFSET_NEG, -128, 128, pageCalibration, FuncActive, OnChange_NegativeAD9952, EmptyFuncVV
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_PositiveAD9952()
{

}

DEF_GOVERNOR( gPositiveAD9952,                                                                                                         //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА - AD9952 +5В ---
    "AD9952 +5В", "AD9952 +5V",
    "Настройка уровня +5В AD9952",
    "Level settings +5V AD9952",
    CAL_AD9952_OFFSET_POS, -128, 128, pageCalibration, FuncActive, OnChange_PositiveAD9952, EmptyFuncVV
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_ZeroAD9952()
{

}

DEF_GOVERNOR( gZeroAD9952,                                                                                                              //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА - AD9952 0В ---
    "AD9952 0В", "AD9952 0V",
    "Настройка уровня +5В AD9952",
    "Level settings +5V AD9952",
    CAL_AD9952_OFFSET_ZERO, -128, 128, pageCalibration, FuncActive, OnChange_ZeroAD9952, EmptyFuncVV
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_AmplitudeAD9952()
{

}

DEF_GOVERNOR( gAmplitudeAD9952,                                                                                                     //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА - AD9952 Размах ---
    "AD9952 Размах", "AD9952 Amplitude",
    "Настройка размаха AD9952",
    "Amplitude settings AD9952",
    CAL_AD9952_AMPLITUDE, -128, 128, pageCalibration, FuncActive, OnChange_AmplitudeAD9952, EmptyFuncVV
)



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4( pageCalibration,                                                                                                                        //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА ---
    "КАЛИБРОВКА", "CALIBRATION",
    "", "",
    &gPositiveAD9952,       ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА - AD9952 +5В
    &gZeroAD9952,           ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА - AD9952 0В
    &gNegativeAD9952,       ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА - AD9952 -5В
    &gAmplitudeAD9952,      ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА - AD9952 размах
    Page::Settings_Calibration, PageSignals::pointer, FuncActive, FuncPress, FuncOnKey, FuncDrawPage
)
