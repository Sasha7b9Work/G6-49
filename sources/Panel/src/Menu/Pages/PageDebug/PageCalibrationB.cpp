#include "Menu/MenuItems.h"
#include "Menu/Pages/PageDebug/PageDebug.h"
#include "Settings/CalibrationSettings.h"


extern const PageBase pageCalibrationB;
Page *PageDebug::PageCalibration::PageB::pointer = reinterpret_cast<Page *>(const_cast<PageBase *>(&pageCalibrationB));



static void OnChange_NegativeAD9952()
{
    PageDebug::PageCalibration::WriteKoeffCal(Chan::B, KoeffCal::AD9952_NEG);
}

static void OnPress_NegativeAD9952(bool enter)
{
    PageDebug::PageCalibration::OnPress_OffsetAD9952(Chan::B, enter, KoeffCal::AD9952_NEG);
}

DEF_GOVERNOR( gNegativeAD9952,                                                                                                       //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - AD9952 -5В ---
    "AD9952 -5В",
    "Настройка уровня -5В AD9952",
    CAL_AD9952_OFFSET_NEG(Chan::B), 3000, 4095, pageCalibrationB, Item::FuncActive, OnChange_NegativeAD9952, FuncBeforeDraw, OnPress_NegativeAD9952
)



static void OnChange_PositiveAD9952()
{
    PageDebug::PageCalibration::WriteKoeffCal(Chan::B, KoeffCal::AD9952_POS);
}

static void OnPress_PositiveAD9952(bool enter)
{
    PageDebug::PageCalibration::OnPress_OffsetAD9952(Chan::B, enter, KoeffCal::AD9952_POS);
}

DEF_GOVERNOR( gPositiveAD9952,                                                                                                       //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - AD9952 +5В ---
    "AD9952 +5В",
    "Настройка уровня +5В AD9952",
    CAL_AD9952_OFFSET_POS(Chan::B), 0, 1000, pageCalibrationB, Item::FuncActive, OnChange_PositiveAD9952, FuncBeforeDraw, OnPress_PositiveAD9952
)



static void OnChange_ZeroAD9952()
{
    PageDebug::PageCalibration::WriteKoeffCal(Chan::B, KoeffCal::AD9952_ZERO);
}

static void OnPress_ZeroAD9952(bool enter)
{
    PageDebug::PageCalibration::OnPress_OffsetAD9952(Chan::B, enter, KoeffCal::AD9952_ZERO);
}

DEF_GOVERNOR( gZeroAD9952,                                                                                                            //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - AD9952 0В ---
    "AD9952 0В",
    "Настройка уровня +5В AD9952",
    CAL_AD9952_OFFSET_ZERO(Chan::B), 1000, 3000, pageCalibrationB, Item::FuncActive, OnChange_ZeroAD9952, FuncBeforeDraw, OnPress_ZeroAD9952
)



static void OnChange_AmplitudeAD9952()
{
    PageDebug::PageCalibration::WriteKoeffCal(Chan::B, KoeffCal::AD9952_AMPL);
}

static void OnPress_AmplitudeAD9952(bool enter)
{
    PageDebug::PageCalibration::OnPress_AmplitudeAD9952(Chan::B, enter, KoeffCal::AD9952_AMPL);
}

DEF_GOVERNOR( gAmplitudeAD9952,                                                                                                   //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - AD9952 Размах ---
    "AD9952 Размах",
    "Настройка размаха AD9952",
    CAL_AD9952_AMPLITUDE(Chan::B), -2000, 2000, pageCalibrationB, Item::FuncActive, OnChange_AmplitudeAD9952, FuncBeforeDraw, OnPress_AmplitudeAD9952
)



static void OnChange_OffsetDDS()
{
    PageDebug::PageCalibration::WriteKoeffCal(Chan::B, KoeffCal::DDS_OFFSET);
}

static void OnPress_OffsetDDS(bool enter)
{
    PageDebug::PageCalibration::OnPress_DDS(Chan::B, enter, KoeffCal::DDS_OFFSET);
}

DEF_GOVERNOR( gOffsetDDS,                                                                                                                //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - DDS 0В ---
    "DDS 0В",
    "Установка нулевого смещения формирователя произвольных сигналов",
    CAL_DDS_OFFSET(Chan::B), 1000, 3000, pageCalibrationB, Item::FuncActive, OnChange_OffsetDDS, FuncBeforeDraw, OnPress_OffsetDDS
)



static void OnChange_MinDDS()
{
    PageDebug::PageCalibration::WriteKoeffCal(Chan::B, KoeffCal::DDS_MIN);
}

static void OnPress_MinDDS(bool enter)
{
    PageDebug::PageCalibration::OnPress_DDS(Chan::B, enter, KoeffCal::DDS_MIN);
}

DEF_GOVERNOR( gMinDDS,                                                                                                                  //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - DDS -5В ---
    "DDS -5В",
    "Установка минимального выходного уровня формирователя произвольных сигналов",
    CAL_DDS_MIN(Chan::B), -10000, 10000, pageCalibrationB, Item::FuncActive, OnChange_MinDDS, FuncBeforeDraw, OnPress_MinDDS
)


static void OnChange_MaxDDS()
{
    PageDebug::PageCalibration::WriteKoeffCal(Chan::B, KoeffCal::DDS_MAX);
}

static void OnPress_MaxDDS(bool enter)
{
    PageDebug::PageCalibration::OnPress_DDS(Chan::B, enter, KoeffCal::DDS_MAX);
}

DEF_GOVERNOR( gMaxDDS,                                                                                                                  //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - DDS +5В ---
    "DDS +5В",
    "Установка минимального выходного уровня формирователя произвольных сигналов",
    CAL_DDS_MAX(Chan::B), -1000, 0, pageCalibrationB, Item::FuncActive, OnChange_MaxDDS, FuncBeforeDraw, OnPress_MaxDDS
)



static void OnChange_TrigLev()
{
    PageDebug::PageCalibration::WriteKoeffCal(Chan::A, KoeffCal::FREQ_LEVEL_TRIG);
}

DEF_GOVERNOR( gTrigLev,                                                                                                             //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - Ур-нь синхр ---
    "Ур-нь синхр",
    "Установка уровня синхронизации частотомера",
    CAL_FREQ_LEVEL_TRIG, -10000, 10000, pageCalibrationB, Item::FuncActive, OnChange_TrigLev, FuncBeforeDraw, EmptyFuncVB
)




DEF_PAGE_8( pageCalibrationB,                                                                                                                     //--- НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B --- 
    "КАЛИБРОВКА B",
    ""
    ,
    &gPositiveAD9952,       ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - AD9952 +5В
    &gZeroAD9952,           ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - AD9952 0В
    &gNegativeAD9952,       ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - AD9952 -5В
    &gAmplitudeAD9952,      ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - AD9952 размах
    &gOffsetDDS,            ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - DDS 0В
    &gMinDDS,               ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - DDS -5В
    &gMaxDDS,               ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - DDS +5В
    &gTrigLev,              ///< НАСТРОЙКИ СИГНАЛОВ - КАЛИБРОВКА B - Ур-нь синхр
    Page::Settings_CalibrationB, PageSignals::pointer, Item::FuncActive, FuncPress, FuncOnKey, Page::FuncDraw
)
