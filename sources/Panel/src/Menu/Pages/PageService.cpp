#pragma once
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Generator.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "PageService.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pService;
Page *PageService::pointer = (Page *)&pService;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Reset(void)
{
    Generator::Reset();
    Timer::PauseOnTime(100);
    Settings::Load(true);
}

DEF_BUTTON(bReset,                                                                                                           //--- ОТЛАДКА - СБРОС ---
           "СБРОС", "RESET",
           "Сброс настроек на значения по умолчанию",
           "Resetting settings to default values",
           pService, FuncActive, OnPress_Reset, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cService_Language,                                                                                               //--- СЕРВИС - ЯЗЫК ---
    "ЯЗЫК", "LANGUAGE",
    "Выбор языка меню",
    "Selecting the menu language",
    "РУССКИЙ",    "RUSSIAN", "Язык меню - русский",
                             "Menu language - Russian",
    "АНГЛИЙСКИЙ", "ENGLISH", "Язык меню - английский",
                             "Menu language - English",
    FLAG_2, LANG, pService, FuncActive, FuncChangedChoice, FuncDraw
)


//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void OnChanged_Background(bool)
{
    Color::InitGlobalColors();
}

DEF_CHOICE_2( cService_Background,                                                                                        //--- СЕРВИС - ЦВЕТ ФОНА ---
    "ЦВЕТ ФОНА", "BACKGROUND",
    "Выбор цвета фона",
    "Select background color",
    "БЕЛЫЙ",  "WHITE", "Цвет фона - белый",
                       "Background color - white",
    "ЧЁРНЫЙ", "BLACK", "Цвет фона - чёрный",
                       "Background color - black",
    BACKGROUND_BLACK, pService, FuncActive, OnChanged_Background, FuncDraw
)
*/


//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_2( pService,                                                                                                                 //--- СЕРВИС ---
    "СЕРВИС", "SERVICE",
    "Сервисные функции",
    "Service functions",
    bReset,
    cService_Language,   ///< СЕРВИС - ЯЗЫК
    //cService_Background, ///< СЕРВИС - ЦВЕТ ФОНА
    Page_Service, 0, FuncActive, FuncPress
)
