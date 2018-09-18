#pragma once
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Menu
{
public:

    static const int NUM_ITEMS_ON_PAGE = 4;

    static void Init();
        
    static void Update();

    static void Draw();
    /// Возвращает true, если ручка управляет переклюичением подстраниц меню
    static bool RegIsControlSubPages();
    /// Возвращает адрес открытого элемента меню
    static Item *GetOpenedItem();

    static void SetOpenedItem(Item *item);
    /// Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней
    static Item *CurrentItem();
    ///\brief  Здесь хранится адрес элемента меню, соответствующего функциональной клавише [1..5], если она находится в нижнем положении, и 0, 
    /// если ни одна кнопка не нажата.
    static Item *itemUnderKey;
    /// Возвращает позицию страницы на страницы. Только если страница принадлежит главному меню. Иначе -1
    static int GetPosition(Page *page);
    /// Указатель на главную страницу
    static Page *mainPage;
private:
    /// Наризовать загол
    static int DrawTitle();

    static void DrawPagesUGO(int x);

    static void ProcessControl(Control control);
    /// Если какой-то итем раскрыт, то здесь его адрес
    static Item *openedItem_;
};
