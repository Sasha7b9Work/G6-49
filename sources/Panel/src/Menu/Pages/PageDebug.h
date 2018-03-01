#pragma once
#include "Settings/Settings.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void  OnPress_SaveSettings();
static void  OnPress_Reset();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ ---------------------------------------------------------------------------------------------------------------------------------
static const Choice cConsole =
{
    Item_Choice, {}, 0,
    {
        " ŒÕ—ŒÀ‹", "CONSOLE"
    },
    {
        DISABLED_RU, DISABLED_EN,
        ENABLED_RU, ENABLED_EN
    },
    (uint8*)&CONSOLE_ENABLED
};

static void OnPress_DebugMode()
{
    Generator::SetDebugMode(DEBUG_MODE_ENABLED);
}

// Œ“À¿ƒ ¿ - —Œ’–¿Õ»“‹ Õ¿—“–Œ… » ---------------------------------------------------------------------------------------------------------------------
static const Button bSaveSettings =
{
    Item_Button, {}, OnPress_SaveSettings,
    {
        "—Œ’–¿Õ»“‹ Õ¿—“–Œ… »", "SAVE SETTINGS"
    }
};

static void OnPress_SaveSettings(void)
{
    Settings_Save();
}

// Œ“À¿ƒ ¿ - —¡–Œ— -----------------------------------------------------------------------------------------------------------------------------------
static const Button bReset =
{
    Item_Button, {}, OnPress_Reset,
    {
        "—¡–Œ—", "RESET"
    }
};

static void OnPress_Reset(void)
{
    Generator::Reset();
    uint time = TIME_MS;
    while (TIME_MS - time < 100) {};
    Settings_Load();
}

const Page pDebug =
{
    Item_Page, {}, 0,
    {
        "Œ“À¿ƒ ¿", "DEBUG"
    },
    {
        (void *)&cConsole,      ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹
        (void *)&bSaveSettings, ///< Œ“À¿ƒ ¿ - —Œ’–¿Õ»“‹ Õ¿—“–Œ… »
        (void *)&bReset         ///< Œ“À¿ƒ ¿ - —¡–Œ—
    }
};

// Œ“À¿ƒ ¿ - –≈∆»Ã -----------------------------------------------------------------------------------------------------------------------------------
static const Choice cDebugMode =
{
    Item_Choice, {}, OnPress_DebugMode,
    {
        "–≈∆»Ã", "MODE"
    },
    {
        "ÕŒ–Ã¿À‹Õ€…", "NORMAL",
        "Œ“À¿ƒŒ◊Õ€…", "DEBUG"
    },
    (uint8 *)&DEBUG_MODE_ENABLED
};
