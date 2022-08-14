// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Display/Painter.h"
#include "FDrive/FDrive_p.h"


extern const PageBase pageLoad;
Page *PageLoadForm::self = (Page *)&pageLoad;


DEF_BUTTON( bUp,                                                                                                                              //--- НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА - Вверх ---
    "Вверх", "Up",
    "", "",
    pageLoad, Item::EFuncActive, FDrive::PressUp, Item::EFuncDraw
)

volatile const ButtonBase *pbUp = &bUp;


DEF_BUTTON( bDown,                                                                                                                             //--- НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА - Вниз ---
    "Вниз", "Down",
    "", "",
    pageLoad, Item::EFuncActive, FDrive::PressDown, Item::EFuncDraw
)

volatile const ButtonBase *pbDown = &bDown;


DEF_BUTTON( bEnter,                                                                                                                           //--- НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА - Enter ---
    "Выбор", "Choice",
    "", "",
    pageLoad, Item::EFuncActive, Button::FuncPress, Item::EFuncDraw
)

volatile const ButtonBase *pbEnter = &bEnter;


static void OnPress_Choose()
{
    FDrive::PressChoose();
    PageSignals::OnChanged_Form();
}

DEF_BUTTON( bChoose,                                                                                                                     //--- НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА - Установить ---
    "Выбрать", "Select",
    "", "",
    pageLoad, Item::EFuncActive, OnPress_Choose, Item::EFuncDraw
)



static bool OnControl(const Control &control)
{
    if (control.Is(Key::RotateLeft))
    {
        FDrive::PressDown();
        return true;
    }
    else if (control.Is(Key::RotateRight))
    {
        FDrive::PressUp();
        return true;
    }
    else if (control.Is(Key::Esc, Action::Up))
    {
        Menu::ResetAdditionPage();
        return true;
    }

    return false;
}

DEF_PAGE_4( pageLoad,                                                                                                                         //--- НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА --- //-V641
    "ВЫБОР", "CHOICE",   //-V641
    "Страница выбора формы произвольного сигнала для загрузки с флешки", "The page for choosing the shape of an arbitrary waveform to boot from a USB flash drive",
    &bChoose,   // НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА - Установить
    0,
    0,
    0,
    Page::Settings_Load, nullptr, Item::EFuncActive, Page::EFuncEnter, OnControl, FDrive::Draw
)


void PageLoadForm::LoadForm(const Chan &)
{
    Menu::SetAdditionPage(self);
}
