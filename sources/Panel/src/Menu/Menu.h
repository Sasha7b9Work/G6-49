#pragma once
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_PAGES     9        ///< Количество страниц в меню
#define PAGE_IS_MAIN  false
#define MENU_TEMP_ENABLE_STRING_NAVI()

extern const Page *pages[NUM_PAGES];


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
    static Item *OpenedItem();
    /// Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней
    static Item *CurrentItem();
    ///\brief  Здесь хранится адрес элемента меню, соответствующего функциональной клавише [1..5], если она находится в нижнем положении, и 0, 
    /// если ни одна кнопка не нажата.
    static Item *itemUnderKey;
    /// Возвращает адрес текущую страницу
    static Page* CurrentPage();
private:

    static void DrawTitle();

    static void DrawPagesUGO();

    static void ProcessControl(StructControl control);

    static Item *openedItem;
};
