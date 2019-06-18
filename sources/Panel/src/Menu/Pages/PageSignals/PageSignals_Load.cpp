#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Menu/Pages/Include/PageSignals.h"
#include "Display/Painter.h"
#include "FDrive/FDrivePanel.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageLoad;
Page *PageSignals::PageLoad::pointer = (Page *)&pageLoad;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_BUTTON( bUp,                                                                                                                              //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - «¿√–”« ¿ - ¬‚Âı ---
    "¬‚Âı",
    "",
    pageLoad, Item::FuncActive, FDrive::PressUp, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON( bDown,                                                                                                                             //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - «¿√–”« ¿ - ¬ÌËÁ ---
    "¬ÌËÁ",
    "",
    pageLoad, Item::FuncActive, FDrive::PressDown, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON( bEnter,                                                                                                                           //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - «¿√–”« ¿ - Enter ---
    "¬˚·Ó",
    "",
    pageLoad, Item::FuncActive, FuncPress, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Choose()
{
    FDrive::PressChoose();
    PageSignals::OnPress_Form(true);
}

DEF_BUTTON( bChoose,                                                                                                                     //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - «¿√–”« ¿ - ”ÒÚ‡ÌÓ‚ËÚ¸ ---
    "¬˚·‡Ú¸",
    "",
    pageLoad, Item::FuncActive, OnPress_Choose, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool OnKey(Control event)
{
    if (event.value == Control::RegLeft)
    {
        FDrive::PressDown();
        return true;
    }
    else if (event.value == Control::RegRight)
    {
        FDrive::PressUp();
        return true;
    }
    else
    {
        // Á‰ÂÒ¸ ÌË˜Â„Ó
    }

    return false;
}

DEF_PAGE_4( pageLoad, //-V641 //-V1027                                                                                                                //--- Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - «¿√–”« ¿ ---
    "«¿√–”« ¿",
    "",
    &bChoose,   ///< Õ¿—“–Œ… » —»√Õ¿ÀŒ¬ - «¿√–”« ¿ - ”ÒÚ‡ÌÓ‚ËÚ¸
    0,
    0,
    0,
    Page::Settings_Load, PageSignals::pointer, Item::FuncActive, FuncPress, OnKey, FDrive::Draw
)
