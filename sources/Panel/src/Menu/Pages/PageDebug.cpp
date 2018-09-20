#include "PageDebug.h"
#include "Generator/Generator.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Display/font/Font.h"
#include "PageService.h"


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
    DISABLED_RU, DISABLED_EN, "Отображение консоли включено",
                              "Console display enabled",
    ENABLED_RU,  ENABLED_EN,  "Отображение консоли выключено",
                              "Console display disabled",
    FLAG_1, BIT_CONSOLE, pDebug, FuncActive, OnPress_DebugMode, FuncDraw
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cStatistics,                                                                                              //--- ОТЛАДКА - Статистика ---
    "СТАТИСТИКА", "STATISTICS",
    "Включает/отключает показ на экране статистики по частоте кадров в виде ФПС / время отрисовки одного кадра / время, потраченое на отрисовку за "
    "весь кадр",
    "Enables / disables the display of statistics on the frame rate in the form of FPS / time of drawing one frame / time spent on rendering for the "
    "whole frame",
    DISABLED_RU, DISABLED_EN, "Показ статистики включён",
                              "Statistics show on",
    ENABLED_RU, ENABLED_EN,   "Показ статистики выключен",
                              "Statistics showing off",
    FLAG_1, BIT_STATISTICS, pDebug, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_BigSymbols(bool)
{
    Font::ToggleCharacterSize();
}

DEF_CHOICE_2 (  cBigSymbols,                                                                                       //--- ОТЛАДКА - Большие символы ---
    "Большие символы", "Large characters",
    "Отображение информации более крупными символами",
    "Display information with larger characters",
    DISABLED_RU, DISABLED_EN,   "Отображение информации стандартными символами",
                                "Displaying information with standard symbols",
    ENABLED_RU, ENABLED_EN,     "Отображение информации увеличенными символами",
                                "Displaying information with enlarged symbols",
    FLAG_1, BIT_BIG_SYMBOLS, pDebug, FuncActive, OnPress_BigSymbols, FuncDraw
)


DEF_PAGE_3( pDebug,
    "ОТЛАДКА", "DEBUG",
    "", "",
    cConsole,       ///< ОТЛАДКА - Консоль
    bSaveSettings,  ///< ОТЛАДКА - Сохранить настройки
    cStatistics,    ///< ОТЛАДКА - Статистика
//    cBigSymbols,    ///< ОТЛАДКА - Больщие символы
    Page::Debug, PageService::pointer, FuncActive, FuncPress, FuncOnKey
)
