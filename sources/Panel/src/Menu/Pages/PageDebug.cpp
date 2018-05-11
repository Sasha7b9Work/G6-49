#include "PageDebug.h"
#include "Generator.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pDebug;
Page *PageDebug::pointer = (Page *)&pDebug;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_DebugMode(bool)
{
    Generator::SetDebugMode(DEBUG_MODE_ENABLED);
}

DEF_CHOICE_2(cConsole,                                                                                                     //--- ОТЛАДКА - КОНСОЛЬ ---
    "КОНСОЛЬ", "CONSOLE",
    "Включает и выключает отображение отладочной консоли",
    "Turns on or off the display of the debug console",
    DISABLED_RU, DISABLED_EN,
    ENABLED_RU,  ENABLED_EN,
    CONSOLE_ENABLED, pDebug, FuncActive, OnPress_DebugMode, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SaveSettings()
{
    Settings::Save();
}

DEF_BUTTON( bSaveSettings,                                                                                     //--- ОТЛАДКА - СОХРАНИТЬ НАСТРОЙКИ ---
    "СОХРАНИТЬ НАСТРОЙКИ", "SAVE SETTINGS",
    "Сохранить текущие настройки",
    "Save current settings",
    pDebug, FuncActive, OnPress_SaveSettings, FuncDraw
)

DEF_PAGE_2( pDebug,
    "ОТЛАДКА", "DEBUG",
    "", "",
    cConsole,      ///< ОТЛАДКА - КОНСОЛЬ
    bSaveSettings, ///< ОТЛАДКА - СОХРАНИТЬ НАСТРОЙКИ
    Page_Debug, 0, FuncActive, FuncPress
)
