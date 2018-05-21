#pragma once
#include "PageSettings2.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pSignals2;
Page *PageSignals2::pointer = (Page *)&pSignals2;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cTypeTune,                                                                                      //--- НАСТРОЙКИ СИГНАЛОВ 2 - ЗАСЫЛКА ---
    "ЗАСЫЛКА", "SENDING",
    "Определяет периодичность засылки настроек сигналов в устройство управления",
    "Specifies the frequency of sending signal settings to the control device",
    "ОДНОКРАТНО", "SINGLE",       "Засылка настроек сигнала происходит однократно, после нажатия кнопки подтверждения",
                                  "The signal settings are sent once, after you press the confirm button",
    "НЕПРЕРЫВНО", "CONTINUOUSLY", "Засылка настроек сигнала происходит синхронно с изменением информации на экране",
                                  "Sending of signal settings occurs synchronously with changing information on the screen",
    set.sig_tuneFull, pSignals2, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_1( pSignals2,                                                                                                  //--- НАСТРОЙКИ СИГНАЛОВ 2 ---
    "НАСТРОЙКИ СИГНАЛОВ 2", "SIGNAL SETTINGS 2",
    "",
    "",
    cTypeTune,                                      ///< НАСТРОЙКИ СИГНАЛОВ 2 - ЗАСЫЛКА
    Page_Settings2, 0, FuncActive, FuncPress
)
