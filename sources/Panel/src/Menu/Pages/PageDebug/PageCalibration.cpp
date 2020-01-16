#include "defines.h"
#include "Menu/Pages/PageDebug/PageDebug.h"



DEF_PAGE_2(pCalibration,
    " ¿À»¡–Œ¬ ¿",
    "",
    PageDebug::_Calibration::PageA::self, ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ A
    PageDebug::_Calibration::PageB::self, ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ -  ¿À»¡–Œ¬ ¿ B
    Page::Calibration, PageDebug::self, Item::FuncActive, FuncPress, FuncOnKey, Page::FuncDraw
)

Page *PageDebug::Calibartion::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pCalibration));
