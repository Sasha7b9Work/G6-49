#pragma once
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include "PageService.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pService;
Page *PageService::pointer = (Page *)&pService;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cService_Language,                                                                                               //--- СЕРВИС - ЯЗЫК ---
    "ЯЗЫК", "LANGUAGE",
    "Выбор языка меню",
    "Selecting the menu language",
    "РУССКИЙ",    "RUSSIAN",
    "АНГЛИЙСКИЙ", "ENGLISH",
    LANG, pService, FuncActive, FuncChangedChoice, FuncDraw
)


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Background(bool)
{
    Color::InitGlobalColors();
}

DEF_CHOICE_2( cService_Background,                                                                                        //--- СЕРВИС - ЦВЕТ ФОНА ---
    "ЦВЕТ ФОНА", "BACKGROUND",
    "Выбор цвета фона",
    "Select background color",
    "БЕЛЫЙ",  "WHITE",
    "ЧЁРНЫЙ", "BLACK",
    BACKGROUND_BLACK, pService, FuncActive, OnChanged_Background, FuncDraw
)


//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_2( pService,                                                                                                                 //--- СЕРВИС ---
    "СЕРВИС", "SERVICE",
    "Сервисные функции",
    "Service functions",
    cService_Language,   ///< СЕРВИС - ЯЗЫК
    cService_Background, ///< СЕРВИС - ЦВЕТ ФОНА
    Page_Service, 0, FuncActive, FuncPress
)
