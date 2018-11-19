#include "stdafx.h"
#ifndef WIN32
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageSignals.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageCalibration;
Page *PageSignals::PageCalibration::pointer = (Page *)&pageCalibration;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4( pageCalibration,
    " ¿À»¡–Œ¬ ¿", "CALIBRATION",
    "", "",
    0,      ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ - —ËÌÛÒ +5¬
    0,      ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ - —ËÌÛÒ 0¬
    0,      ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ - —ËÌÛÒ -5¬
    0,      ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ - —ËÌÛÒ ‡ÁÏ‡ı
    Page::Settings_Calibration, PageSignals::pointer, FuncActive, FuncPress, FuncOnKey, FuncDrawPage
)
