#include "PageUSB.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pUSB;
Page *PageUSB::pointer = (Page *)&pUSB;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cSizeByte,                                                                                                   //--- USB - РАЗМЕР БАЙТА ---
   "РАЗМЕР БАЙТА", "SIZE BYTE",
   "", "",
   "7 бит", "7 bits", "7 бит в байте",
                      "7 bits in byte",
   "8 бит", "8 bits", "8 бит в байте",
                      "8 bits in byte",
   FLAG_2, BIT_SIZE_BYTE, pUSB, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cStopBit,                                                                                                        //--- USB - СТОП-БИТ ---
    "СТОП-БИТ", "STOP-BIT",
    "", "",
    "1", "1", "1 стоп-бит в конце байта",
              "1 stop bit at the end of the byte",
    "2", "2", "2 стоп-бита в конце байта",
              "2 stop bits at the end of the byte",
    FLAG_2, BIT_STOP_BIT, pUSB, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cParity,                                                                                                         //--- USB - ЧЁТНОСТЬ ---
    "ЧЁТНОСТЬ", "PARITY",
    "", "",
    "ПРОВЕРЯТЬ", "VERIFY",        "Проверка чётности включена",
                                  "Parity check enabled",
    "НЕ ПРОВЕРЯТЬ", "NOT VERIFY", "Проверка чётности выключена",
                                  "Parity check off",
    FLAG_1, BIT_PARITY, pUSB, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3(pUSB,                                                                                                                         //--- USB ---
    "USB", "USB",
    "Настройки параметров связи по интерфейсу USB",
    "Communication parameters settings on the USB interface",
    cSizeByte,              ///< USB - РАЗМЕР БАЙТА
    cStopBit,               ///< USB - СТОП-БИТ
    cParity,                ///< USB - ЧЁТНОСТЬ
    Page_USB, 0, FuncActive, FuncPress
)
