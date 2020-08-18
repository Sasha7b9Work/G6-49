#include "defines.h"
#include "log.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Display/Painter.h"
#include "FDrive/FDrive_p.h"


extern const PageBase pageLoad;
Page *PageLoadForm::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pageLoad));


DEF_BUTTON( bUp,                                                                                                                              //--- НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА - Вверх ---
    "Вверх", "Up",
    "", "",
    pageLoad, Item::FuncActive, FDrive::PressUp, FuncDraw
)

volatile const ButtonBase *pbUp = &bUp;


DEF_BUTTON( bDown,                                                                                                                             //--- НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА - Вниз ---
    "Вниз", "Down",
    "", "",
    pageLoad, Item::FuncActive, FDrive::PressDown, FuncDraw
)

volatile const ButtonBase *pbDown = &bDown;


DEF_BUTTON( bEnter,                                                                                                                           //--- НАСТРОЙКИ СИГНАЛОВ - ЗАГРУЗКА - Enter ---
    "Выбор", "Choice",
    "", "",
    pageLoad, Item::FuncActive, Button::FuncPress, FuncDraw
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
    pageLoad, Item::FuncActive, OnPress_Choose, FuncDraw
)



static bool OnControl(const Control control) //-V801
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
    Page::Settings_Load, nullptr, Item::FuncActive, Page::FuncEnter, OnControl, FDrive::Draw
)


void PageLoadForm::LoadForm(Chan::E)
{
    Menu::SetAdditionPage(self);
}
