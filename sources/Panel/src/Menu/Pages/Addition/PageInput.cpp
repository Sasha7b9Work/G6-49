#include "stdafx.h"

#ifndef WIN32
#include "Display/InputWindow.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Generator/Signals.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "PageInput.h"
#endif



extern const PageBase pInput;



static void Draw_Left(int x, int y)
{
    if (FORM_CURRENT->CurrentParameter()->AssumeArbitaryOrder())
    {
        String("Порядок").Draw(x + 14, y + 12);
        Text::Draw4SymbolsInRect(x + 26, y + 30, Ideograph::_8::FillUp);
    }
}

DEF_SMALL_BUTTON(sbOrderUp,                                                                                           //--- ОКНО ВВОДА - СИМВОЛ ВЛЕВО ---
    "Порядок больше",
    "Увеличить порядок единицы измерения",
    pInput, Item::FuncActive, InputWindow::OrderUp, Draw_Left
)


static void Draw_Right(int x, int y)
{
    if (FORM_CURRENT->CurrentParameter()->AssumeArbitaryOrder())
    {
        String("Порядок").Draw(x + 14, y + 12);
        Text::Draw4SymbolsInRect(x + 26, y + 30, Ideograph::_8::FillDown);
    }
}

DEF_SMALL_BUTTON(sbOrderDown,                                                                                         //--- ОКНО ВВОДА - СИМВОЛ ВПРАВО ---
    "Порядок меньшше",
    "Уменьшить порядок единицы измерения",
    pInput, Item::FuncActive, InputWindow::OrderDown, Draw_Right
)



static void OnPress_Cancel()
{
    Menu::ResetAdditionPage();
}

static void Draw_Cancel(int x, int y)
{
    String("Отменить").Draw(x + 13, y + 12);
    Text::Draw4SymbolsInRect(x + 25, y + 28, Ideograph::_8::Delete);
}

DEF_SMALL_BUTTON(sbCancel,                                                                                               //--- ОКНО ВВОДА - ОТМЕНА ---
    "ЕДИНИЦЫ ИЗМЕРЕНИЯ",
    "Отказаться от ввода нового значения",
    pInput, Item::FuncActive, OnPress_Cancel, Draw_Cancel
)


static void Draw_Enter(int x, int y)
{
    String("Подтвердить").Draw(x + 3, y + 12);
    Text::Draw4SymbolsInRect(x + 25, y + 28, Ideograph::_8::Save);
}

DEF_SMALL_BUTTON(sbEnter,                                                                                                  //--- ОКНО ВВОДА - ВВОД ---
    "ВВОД",
    "Подтвердить ввод нового значения",
    pInput, Item::FuncActive, InputWindow::KeyEnter, Draw_Enter
)


static bool OnControl(KeyEvent &key)
{
    if(!key.IsFunctional())
    {
        InputWindow::ProcessContorl(key);
        return true;
    }
    return false;
}

Page *PageInput::pointer = reinterpret_cast<Page *>(const_cast<PageBase *>(&pInput));

DEF_PAGE_SB( pInput, //-V641 //-V1027
    "ВВОД ЗНАЧЕНИЯ",
    "",
    &sbOrderUp,         ///< ОКНО ВВОДА - СИМВОЛ ВЛЕВО
    &sbOrderDown,       ///< ОКНО ВВОДА - СИМВОЛ ВПРАВО
    &sbCancel,          ///< ОКНО ВВОДА - ОТМЕНА
    &sbEnter,           ///< ОКНО ВВОДА - ВВОД
    0,
    0,
    Page::SB_Input, 0, Item::FuncActive, FuncPress, Page::FuncDraw, OnControl
)
