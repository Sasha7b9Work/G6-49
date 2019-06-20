#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageSignals.h"
#include "Display/Painter.h"
#include "FDrive/FDrivePanel.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageLoad;
Page *PageSignals::PageLoad::pointer = (Page *)&pageLoad;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_BUTTON( bUp,                                                                                                                              //--- НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА - Вверх ---
    "Вверх",
    "",
    pageLoad, Item::FuncActive, FDrive::PressUp, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON( bDown,                                                                                                                             //--- НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА - Вниз ---
    "Вниз",
    "",
    pageLoad, Item::FuncActive, FDrive::PressDown, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON( bEnter,                                                                                                                           //--- НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА - Enter ---
    "Выбор",
    "",
    pageLoad, Item::FuncActive, FuncPress, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Choose()
{
    FDrive::PressChoose();
    PageSignals::OnPress_Form(true);
}

DEF_BUTTON( bChoose,                                                                                                                     //--- НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА - Установить ---
    "Выбрать",
    "",
    pageLoad, Item::FuncActive, OnPress_Choose, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool OnKey(Control &control)
{
    if (control.value == Control::RegLeft)
    {
        FDrive::PressDown();
        return true;
    }
    else if (control.value == Control::RegRight)
    {
        FDrive::PressUp();
        return true;
    }
    else if (control.Is(Control::Esc, Control::Action::Up))
    {
        Menu::ResetAdditionPage();
        return true;
    }
    else
    {
        // здесь ничего
    }

    return false;
}

DEF_PAGE_4( pageLoad, //-V641 //-V1027                                                                                                                //--- НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА ---
    "ВЫБОР",
    "Страница выбора формы произвольного сигнала для загрузки с флешки",
    &bChoose,   ///< НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА - Установить
    0,
    0,
    0,
    Page::Settings_Load, nullptr, Item::FuncActive, FuncPress, OnKey, FDrive::Draw
)
