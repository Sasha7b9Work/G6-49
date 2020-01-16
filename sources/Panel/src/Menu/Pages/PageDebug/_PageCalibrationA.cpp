#include "Menu/MenuItems.h"
#include "Menu/Pages/PageDebug/PageDebug.h"
#include "Menu/Pages/PageSignals/PageSignals.h"
#include "Settings/CalibrationSettings.h"


extern const PageBase pageCalibrationA;
Page *PageDebug::_Calibration::PageA::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pageCalibrationA));



static void OnChange_NegativeAD9952()
{
    PageDebug::_Calibration::WriteKoeffCal(Chan::A, KoeffCal::AD9952_NEG);
}

static void OnPress_NegativeAD9952(bool enter)
{
    PageDebug::_Calibration::OnPress_OffsetAD9952(Chan::A, enter, KoeffCal::AD9952_NEG);
}

DEF_GOVERNOR( gNegativeAD9952,                                                                                                       //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A - AD9952 -5В ---
    "AD9952 -5В",
    "Настройка уровня -5В AD9952",
    CAL_AD9952_OFFSET_NEG(Chan::A), 3000, 4095, pageCalibrationA, Item::FuncActive, OnChange_NegativeAD9952, EmptyFuncVV, OnPress_NegativeAD9952
)



static void OnChange_PositiveAD9952()
{
    PageDebug::_Calibration::WriteKoeffCal(Chan::A, KoeffCal::AD9952_POS);
}

static void OnPress_PositiveAD9952(bool enter)
{
    PageDebug::_Calibration::OnPress_OffsetAD9952(Chan::A, enter, KoeffCal::AD9952_POS);
}

DEF_GOVERNOR( gPositiveAD9952,                                                                                                       //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A - AD9952 +5В ---
    "AD9952 +5В",
    "Настройка уровня +5В AD9952",
    CAL_AD9952_OFFSET_POS(Chan::A), 0, 1000, pageCalibrationA, Item::FuncActive, OnChange_PositiveAD9952, EmptyFuncVV, OnPress_PositiveAD9952
)



static void OnChange_ZeroAD9952()
{
    PageDebug::_Calibration::WriteKoeffCal(Chan::A, KoeffCal::AD9952_ZERO);
}

static void OnPress_ZeroAD9952(bool enter)
{
    PageDebug::_Calibration::OnPress_OffsetAD9952(Chan::A, enter, KoeffCal::AD9952_ZERO);
}

DEF_GOVERNOR( gZeroAD9952,                                                                                                            //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A - AD9952 0В ---
    "AD9952 0В",
    "Настройка уровня +5В AD9952",
    CAL_AD9952_OFFSET_ZERO(Chan::A), 1000, 3000, pageCalibrationA, Item::FuncActive, OnChange_ZeroAD9952, EmptyFuncVV, OnPress_ZeroAD9952
)



static void OnChange_AmplitudeAD9952()
{
    PageDebug::_Calibration::WriteKoeffCal(Chan::A, KoeffCal::AD9952_AMPL);
}

static void OnPress_AmplitudeAD9952(bool enter)
{
    PageDebug::_Calibration::OnPress_AmplitudeAD9952(Chan::A, enter, KoeffCal::AD9952_AMPL);
}

DEF_GOVERNOR( gAmplitudeAD9952,                                                                                                   //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A - AD9952 Размах ---
    "AD9952 Размах",
    "Настройка размаха AD9952",
    CAL_AD9952_AMPLITUDE(Chan::A), -2000, 2000, pageCalibrationA, Item::FuncActive, OnChange_AmplitudeAD9952, EmptyFuncVV, OnPress_AmplitudeAD9952
)


static void OnChange_OffsetDDS()
{
    PageDebug::_Calibration::WriteKoeffCal(Chan::A, KoeffCal::DDS_OFFSET);
}

static void OnPress_OffsetDDS(bool enter)
{
    PageDebug::_Calibration::OnPress_DDS(Chan::A, enter, KoeffCal::DDS_OFFSET);
}

DEF_GOVERNOR( gOffsetDDS,                                                                                                                //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A - DDS 0В ---
    "DDS 0В",
    "Установка нулевого смещения формирователя произвольных сигналов",
    CAL_DDS_OFFSET(Chan::A), 1000, 3000, pageCalibrationA, Item::FuncActive, OnChange_OffsetDDS, FuncBeforeDraw, OnPress_OffsetDDS
)



static void OnChange_MinDDS()
{
    PageDebug::_Calibration::WriteKoeffCal(Chan::A, KoeffCal::DDS_MIN);
}

static void OnPress_MinDDS(bool enter)
{
    PageDebug::_Calibration::OnPress_DDS(Chan::A, enter, KoeffCal::DDS_MIN);
}

DEF_GOVERNOR( gMinDDS,                                                                                                                  //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A - DDS -5В ---
    "DDS -5В",
    "Установка минимального выходного уровня формирователя произвольных сигналов",
    CAL_DDS_MIN(Chan::A), -10000, 10000, pageCalibrationA, Item::FuncActive, OnChange_MinDDS, FuncBeforeDraw, OnPress_MinDDS
)

volatile const GovernorBase *pgMinDDS = &gMinDDS;


static void OnChange_MaxDDS()
{
    PageDebug::_Calibration::WriteKoeffCal(Chan::A, KoeffCal::DDS_MAX);
}

static void OnPress_MaxDDS(bool enter)
{
    PageDebug::_Calibration::OnPress_DDS(Chan::A, enter, KoeffCal::DDS_MAX);
}

DEF_GOVERNOR( gMaxDDS,                                                                                                                  //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A - DDS +5В ---
    "DDS +5В",
    "Установка минимального выходного уровня формирователя произвольных сигналов",
    CAL_DDS_MAX(Chan::A), -1000, 0, pageCalibrationA, Item::FuncActive, OnChange_MaxDDS, FuncBeforeDraw, OnPress_MaxDDS
)


static void OnChange_TrigLev()
{
    PageDebug::_Calibration::WriteKoeffCal(Chan::A, KoeffCal::FREQ_LEVEL_TRIG);
}

DEF_GOVERNOR( gTrigLev,                                                                                                             //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА А - Ур-нь синхр ---
    "Ур-нь синхр",
    "Установка уровня синхронизации частотомера",
    CAL_FREQ_LEVEL_TRIG, -10000, 10000, pageCalibrationA, Item::FuncActive, OnChange_TrigLev, FuncBeforeDraw, EmptyFuncVB
)




DEF_PAGE_7( pageCalibrationA,                                                                                                                     //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A --- 
    "КАЛИБРОВКА A",
    "",
    &gPositiveAD9952,       ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A - AD9952 +5В
    &gZeroAD9952,           ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A - AD9952 0В
    &gNegativeAD9952,       ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A - AD9952 -5В
    &gAmplitudeAD9952,      ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A - AD9952 размах
    &gOffsetDDS,            ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A - DDS 0В
    &gMaxDDS,               ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА A - DDS +5В
    &gTrigLev,              ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА А - Ур-нь синхр
    Page::Settings_CalibrationA, PageDebug::self, Item::FuncActive, FuncPress, FuncOnKey, Page::FuncDraw
)
