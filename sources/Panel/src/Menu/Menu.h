#pragma once
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_PAGES     4        ///< Количество страниц в меню
#define PAGE_IS_MAIN  false
#define MENU_TEMP_ENABLE_STRING_NAVI()

extern const Page *menu[NUM_PAGES];

class Menu
{
public:
        
    static void Init();

    static void Update();

    static void Draw();
    /// Возвращает true, если ручка управляет переклюичением страниц меню
    static bool RegIsControlPages();

    static const char *NameCurrentParameter();
    /// Возвращает адрес открытого элемента меню
    static Control *OpenedItem();
    /// Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней
    static Control *CurrentItem();
    ///\brief  Здесь хранится адрес элемента меню, соответствующего функциональной клавише [1..5], если она находится в нижнем положении, и 0, 
    /// если ни одна кнопка не нажата.
    static Control *itemUnderKey;

    static void SetItemForHint(void *item);

private:

    static void DrawTitle();

    static void DrawPagesUGO();

    static void ProcessControl(StructControl control);
};
