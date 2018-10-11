#pragma once
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Menu
{
public:
    /// Столько элементов меню помещается на дисплее одновременно
    static const int NUM_ITEMS_ON_DISPLAY = 4;

    static void Init();
        
    static void Update();

    static void Draw();
    /// Возвращает true, если ручка управляет переклюичением подстраниц меню
    static bool RegIsControlSubPages();
    /// Возвращает адрес открытого элемента меню
    static Item *GetOpenedItem() { return openedItem; };

    static void SetOpenedItem(Item *item) { openedItem = item; };
    /// Обнулить открытый итем - итем закрыт
    static void ResetOpenedItem() { openedItem = 0; };
    ///\brief  Здесь хранится адрес элемента меню, соответствующего функциональной клавише [F1..F4], если она находится в нижнем положении, и 0, 
    /// если ни одна кнопка не нажата.
    static Item *pressedItem;
    /// Возвращает позицию страницы на страницы. Только если страница принадлежит главному меню. Иначе -1
    static int GetPosition(Page *page);
    /// Устанавливает для отрисовки дополнительноую страницу
    static void SetAdditionPage(Page *page);
    /// Синмает дополнительную страницу с отрисовки
    static void ResetAdditionPage();
    /// Указатель на главную страницу
    static Page *mainPage;
private:
    /// Наризовать загол
    static int DrawTitle();
    /// Обработать управляющее воздействие control
    static void ProcessContorl(Control control);

    static void DrawPagesUGO(int x);
    /// Если какой-то итем раскрыт, то здесь его адрес
    static Item *openedItem;
    /// Сюда помещается указатель на страницу основного меню в случае, если нужна отрисовка дополнительной страницы
    static Page *oldPage;
};
