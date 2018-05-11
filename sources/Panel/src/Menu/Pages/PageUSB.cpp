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

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_1(pUSB,
    "USB", "USB",
    "Настройки параметров связи по интерфейсу USB",
    "Communication parameters settings on the USB interface",
    cSizeByte,
    Page_USB, 0, FuncActive, FuncPress
)
