#include "stdafx.h"
#ifndef WIN32
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageSignals.h"
#include "Settings/CalibrationSettings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageCalibrationB;
Page *PageSignals::PageCalibration::PageB::pointer = (Page *)&pageCalibrationB;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_OffsetAD9952(bool enter, KoeffCal::E koeff)
{
    PageSignals::PageCalibration::OnPress_OffsetAD9952(Chan::B, enter, koeff);
}

static void OnChange_KoeffAD9952(KoeffCal::E koeff)
{
    PageSignals::PageCalibration::WriteKoeffCal(Chan::B, koeff);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_NegativeAD9952()
{
    OnChange_KoeffAD9952(KoeffCal::AD9952_NEG);
}

static void OnPress_NegativeAD9952(bool enter)
{
    OnPress_OffsetAD9952(enter, KoeffCal::AD9952_NEG);
}

DEF_GOVERNOR( gNegativeAD9952,                                                                                                       //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ B - AD9952 -5¬ ---
    "AD9952 -5¬", "AD9952 -5V",
    "Õ‡ÒÚÓÈÍ‡ ÛÓ‚Ìˇ -5¬ AD9952",
    "Level setting -5V AD9952",
    CAL_AD9952_OFFSET_NEG(Chan::B), 3000, 4095, pageCalibrationB, FuncActive, OnChange_NegativeAD9952, EmptyFuncVV, OnPress_NegativeAD9952
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_PositiveAD9952()
{
    OnChange_KoeffAD9952(KoeffCal::AD9952_POS);
}

static void OnPress_PositiveAD9952(bool enter)
{
    OnPress_OffsetAD9952(enter, KoeffCal::AD9952_POS);
}

DEF_GOVERNOR( gPositiveAD9952,                                                                                                       //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ B - AD9952 +5¬ ---
    "AD9952 +5¬", "AD9952 +5V",
    "Õ‡ÒÚÓÈÍ‡ ÛÓ‚Ìˇ +5¬ AD9952",
    "Level settings +5V AD9952",
    CAL_AD9952_OFFSET_POS(Chan::B), 0, 1000, pageCalibrationB, FuncActive, OnChange_PositiveAD9952, EmptyFuncVV, OnPress_PositiveAD9952
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_ZeroAD9952()
{
    OnChange_KoeffAD9952(KoeffCal::AD9952_ZERO);
}

static void OnPress_ZeroAD9952(bool enter)
{
    OnPress_OffsetAD9952(enter, KoeffCal::AD9952_ZERO);
}

DEF_GOVERNOR( gZeroAD9952,                                                                                                            //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ B - AD9952 0¬ ---
    "AD9952 0¬", "AD9952 0V",
    "Õ‡ÒÚÓÈÍ‡ ÛÓ‚Ìˇ +5¬ AD9952",
    "Level settings +5V AD9952",
    CAL_AD9952_OFFSET_ZERO(Chan::B), 1000, 3000, pageCalibrationB, FuncActive, OnChange_ZeroAD9952, EmptyFuncVV, OnPress_ZeroAD9952
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_AmplitudeAD9952()
{
    OnChange_KoeffAD9952(KoeffCal::AD9952_AMPL);
}

static void OnPress_AmplitudeAD9952(bool enter)
{
    PageSignals::PageCalibration::OnPress_AmplitudeAD9952(Chan::B, enter, KoeffCal::AD9952_AMPL);
}

DEF_GOVERNOR( gAmplitudeAD9952,                                                                                                   //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ B - AD9952 –‡ÁÏ‡ı ---
    "AD9952 –‡ÁÏ‡ı", "AD9952 Amplitude",
    "Õ‡ÒÚÓÈÍ‡ ‡ÁÏ‡ı‡ AD9952",
    "Amplitude settings AD9952",
    CAL_AD9952_AMPLITUDE(Chan::B), -2000, 2000, pageCalibrationB, FuncActive, OnChange_AmplitudeAD9952, EmptyFuncVV, OnPress_AmplitudeAD9952
)



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4( pageCalibrationB,                                                                                                                     //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ B ---
    " ¿À»¡–Œ¬ ¿ B", "CALIBRATION B",
    "", "",
    &gPositiveAD9952,       ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ B - AD9952 +5¬
    &gZeroAD9952,           ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ B - AD9952 0¬
    &gNegativeAD9952,       ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ B - AD9952 -5¬
    &gAmplitudeAD9952,      ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ B - AD9952 ‡ÁÏ‡ı
    Page::Settings_CalibrationB, PageSignals::pointer, FuncActive, FuncPress, FuncOnKey, FuncDrawPage
)
