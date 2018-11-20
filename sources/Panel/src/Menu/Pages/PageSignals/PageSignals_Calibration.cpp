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

static void OnPress_NegativeAD9952(bool)
{

}

DEF_GOVERNOR( gNegativeAD9952,                                                                                                         //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ - AD9952 -5¬ ---
    "AD9952 -5¬", "AD9952 -5V",
    "Õ‡ÒÚÓÈÍ‡ ÛÓ‚Ìˇ -5¬ AD9952",
    "Level setting -5V AD9952",
    CAL_AD9952_OFFSET_NEG, -128, 128, pageCalibration, FuncActive, OnChange_NegativeAD9952, EmptyFuncVV, OnPress_NegativeAD9952
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_PositiveAD9952()
{

}

static void OnPress_PositiveAD9952(bool)
{

}

DEF_GOVERNOR( gPositiveAD9952,                                                                                                         //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ - AD9952 +5¬ ---
    "AD9952 +5¬", "AD9952 +5V",
    "Õ‡ÒÚÓÈÍ‡ ÛÓ‚Ìˇ +5¬ AD9952",
    "Level settings +5V AD9952",
    CAL_AD9952_OFFSET_POS, -128, 128, pageCalibration, FuncActive, OnChange_PositiveAD9952, EmptyFuncVV, OnPress_PositiveAD9952
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_ZeroAD9952()
{

}

static void OnPress_ZeroAD9925(bool)
{

}

DEF_GOVERNOR( gZeroAD9952,                                                                                                              //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ - AD9952 0¬ ---
    "AD9952 0¬", "AD9952 0V",
    "Õ‡ÒÚÓÈÍ‡ ÛÓ‚Ìˇ +5¬ AD9952",
    "Level settings +5V AD9952",
    CAL_AD9952_OFFSET_ZERO, -128, 128, pageCalibration, FuncActive, OnChange_ZeroAD9952, EmptyFuncVV, OnPress_ZeroAD9925
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_AmplitudeAD9952()
{

}

static void OnPress_AmplitudeAD9952(bool)
{

}

DEF_GOVERNOR( gAmplitudeAD9952,                                                                                                     //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ - AD9952 –‡ÁÏ‡ı ---
    "AD9952 –‡ÁÏ‡ı", "AD9952 Amplitude",
    "Õ‡ÒÚÓÈÍ‡ ‡ÁÏ‡ı‡ AD9952",
    "Amplitude settings AD9952",
    CAL_AD9952_AMPLITUDE, -128, 128, pageCalibration, FuncActive, OnChange_AmplitudeAD9952, EmptyFuncVV, OnPress_AmplitudeAD9952
)



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4( pageCalibration,                                                                                                                        //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ ---
    " ¿À»¡–Œ¬ ¿", "CALIBRATION",
    "", "",
    &gPositiveAD9952,       ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ - AD9952 +5¬
    &gZeroAD9952,           ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ - AD9952 0¬
    &gNegativeAD9952,       ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ - AD9952 -5¬
    &gAmplitudeAD9952,      ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ - AD9952 ‡ÁÏ‡ı
    Page::Settings_Calibration, PageSignals::pointer, FuncActive, FuncPress, FuncOnKey, FuncDrawPage
)
