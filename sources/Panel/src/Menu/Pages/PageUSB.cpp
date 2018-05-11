#include "PageUSB.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pUSB;
Page *PageUSB::pointer = (Page *)&pUSB;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cSizeByte,
   "РАЗМЕР БАЙТА", "SIZE BYTE",
   "", "",
   "7 бит", "7 bits",
   "8 бит", "8 bits",
   SIZE_BYTE, pUSB, FuncActive, FuncChangedChoice, FuncDraw
)

DEF_CHOICE_2(cStopBit,
    "Стоп-бит", "Stop-bit",
    "", "",
    "1", "1",
    "2", "2",
    STOP_BIT, pUSB, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_2(pUSB,
    "USB", "USB",
    "Настройки параметров связи по интерфейсу USB",
    "Communication parameters settings on the USB interface",
    cSizeByte,              ///< USB - Размер байта
    cStopBit,               ///< USB - Стоп-бит
    Page_USB, 0, FuncActive, FuncPress
)
