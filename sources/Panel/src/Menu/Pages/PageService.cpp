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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pUSB;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pService;
Page *PageService::pointer = (Page *)&pService;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Reset()
{
    Generator::Reset();
    Timer::PauseOnTime(100);
    set.Load(true);
}

DEF_BUTTON(bReset,                                                                                                           //--- ОТЛАДКА - СБРОС ---
    "СБРОС", "RESET",
    "Сброс настроек на значения по умолчанию",
    "Resetting settings to default values",
    pService, Item::FuncActive, OnPress_Reset, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cSizeByte,                                                                                                   //--- USB - РАЗМЕР БАЙТА ---
    "РАЗМЕР БАЙТА", "SIZE BYTE",
    "", "",
    "7 бит", "7 bits", "7 бит в байте",
    "7 bits in byte",
    "8 бит", "8 bits", "8 бит в байте",
    "8 bits in byte",
    FLAG_1, BIT_SIZE_BYTE, pUSB, Item::FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cStopBit,                                                                                                        //--- USB - СТОП-БИТ ---
    "СТОП-БИТ", "STOP-BIT",
    "", "",
    "1", "1", "1 стоп-бит в конце байта",
    "1 stop bit at the end of the byte",
    "2", "2", "2 стоп-бита в конце байта",
    "2 stop bits at the end of the byte",
    FLAG_1, BIT_STOP_BIT, pUSB, Item::FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cParity,                                                                                                         //--- USB - ЧЁТНОСТЬ ---
    "ЧЁТНОСТЬ", "PARITY",
    "", "",
    "ПРОВЕРЯТЬ", "VERIFY", "Проверка чётности включена",
    "Parity check enabled",
    "НЕ ПРОВЕРЯТЬ", "NOT VERIFY", "Проверка чётности выключена",
    "Parity check off",
    FLAG_1, BIT_PARITY, pUSB, Item::FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3(pUSB,                                                                                                                         //--- USB --- //-V641 //-V1027
    "USB", "USB",
    "Настройки параметров связи по интерфейсу USB",
    "Communication parameters settings on the USB interface",
    cSizeByte,              ///< USB - РАЗМЕР БАЙТА
    cStopBit,               ///< USB - СТОП-БИТ
    cParity,                ///< USB - ЧЁТНОСТЬ
    Page::USB, &pService, Item::FuncActive, FuncPress, FuncOnKey
)



extern const PageBase pDebug;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3( pService,                                                                                                                 //--- СЕРВИС --- //-V641 //-V1027
    "СЕРВИС", "SERVICE",
    "Сервисные функции",
    "Service functions",
    bReset,
    pUSB,
    pDebug,
    Page::Service, Menu::mainPage, Item::FuncActive, FuncPress, FuncOnKey
)
