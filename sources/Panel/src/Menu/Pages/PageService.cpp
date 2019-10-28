#include "stdafx.h"
#ifndef WIN32
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Generator/GeneratorPanel.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "PageService.h"
#include "Menu/Menu.h"
#include "Utils/StringUtils.h"
#include "Utils/NumberBuffer.h"
#include "Utils/Math.h"
#include "Display/Symbols.h"
#endif



extern const PageBase pUSB;



extern const PageBase pService;
Page *PageService::pointer = reinterpret_cast<Page *>(const_cast<PageBase *>(&pService));


static void OnPress_Reset()
{
    Generator::Reset();
    Timer::PauseOnTime(100);
    set.Load(true);
}

DEF_BUTTON( bReset,                                                                                                                                                 //--- ОТЛАДКА - СБРОС ---
    "СБРОС",
    "Сброс настроек на значения по умолчанию",
    pService, Item::FuncActive, OnPress_Reset, FuncDraw
)


DEF_CHOICE_2( cSizeByte,                                                                                                                                         //--- USB - РАЗМЕР БАЙТА ---
    "РАЗМЕР БАЙТА",
    ""
    ,
    "7 бит", "7 бит в байте",
    "8 бит", "8 бит в байте",
    FLAG_1, BIT_SIZE_BYTE, pUSB, Item::FuncActive, FuncChangedChoice, FuncDraw
)


DEF_CHOICE_2( cStopBit,                                                                                                                                              //--- USB - СТОП-БИТ ---
    "СТОП-БИТ",
    ""
    ,
    "1", "1 стоп-бит в конце байта",
    "2", "2 стоп-бита в конце байта",
    FLAG_1, BIT_STOP_BIT, pUSB, Item::FuncActive, FuncChangedChoice, FuncDraw
)


DEF_CHOICE_2( cParity,                                                                                                                                               //--- USB - ЧЁТНОСТЬ ---
    "ЧЁТНОСТЬ",
    ""
    ,
    "ПРОВЕРЯТЬ",    "Проверка чётности включена",
    "НЕ ПРОВЕРЯТЬ", "Проверка чётности выключена",
    FLAG_1, BIT_PARITY, pUSB, Item::FuncActive, FuncChangedChoice, FuncDraw
)


DEF_PAGE_3( pUSB, //-V1027                                                                                                                                                      //--- USB ---
    "USB", //-V1027
    "Настройки параметров связи по интерфейсу USB",
    cSizeByte,              ///< USB - РАЗМЕР БАЙТА
    cStopBit,               ///< USB - СТОП-БИТ
    cParity,                ///< USB - ЧЁТНОСТЬ
    Page::USB, &pService, Item::FuncActive, FuncPress, FuncOnKey
)



extern const PageBase pDebug;


DEF_PAGE_3( pService, //-V1027                                                                                                                                               //--- СЕРВИС ---
    "СЕРВИС",  //-V1027
    "Сервисные функции",
    bReset,
    pUSB,
    pDebug,
    Page::Service, Menu::mainPage, Item::FuncActive, FuncPress, FuncOnKey
)
