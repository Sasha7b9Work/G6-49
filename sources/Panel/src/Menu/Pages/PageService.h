#pragma once
#include "Display/Display.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void        OnChanged_Background();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// —≈–¬»— - ﬂ«€  -------------------------------------------------------------------------------------------------------------------------------------
static const Choice cService_Language =
{
    Item_Choice, 0,
    {
        "ﬂ«€ ", "LANGUAGE"
    },
    {
        "–”—— »…", "RUSSIAN",
        "¿Õ√À»…— »…", "ENGLISH"
    },
    (uint8 *)&set.serv_language
};

// —≈–¬»— - ÷¬≈“ ‘ŒÕ¿ --------------------------------------------------------------------------------------------------------------------------------
static const Choice cService_Background =
{
    Item_Choice, OnChanged_Background,
    {
        "÷¬≈“ ‘ŒÕ¿", "BACKGROUND"
    },
    {
        "¡≈À€…",  "WHITE",
        "◊®–Õ€…", "BLACK"
    },
    (uint8 *)&set.serv_bacgroundBlack
};

static void OnChanged_Background(void)
{
    Display_SetColorBackground();
}

const Page pService =
{
    Item_Page, 0,
    {
        "—≈–¬»—", "SERVICE"
    },
    {
        (void*)&cService_Language,      ///< —≈–¬»— - ﬂ«€ 
        (void*)&cService_Background     ///< —≈–¬»— - ÷¬≈“ ‘ŒÕ¿
    }
};
