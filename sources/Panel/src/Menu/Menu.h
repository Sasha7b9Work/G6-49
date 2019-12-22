#pragma once
#include "Menu/MenuItems.h"



struct Menu
{
    /// Столько элементов меню помещается на дисплее одновременно
    static const int NUM_ITEMS_ON_DISPLAY = 4;

    static void Init();
    
    static void Update();

    static void Draw();
    /// Наризовать загол
    static int DrawTitle();

    static void DrawPagesUGO(int x);
    /// Возвращает true, если ручка управляет переклюичением подстраниц меню
    static bool RegIsControlSubPages();
    /// Возвращает адрес открытого элемента меню
    static Item* GetOpenedItem();
    /// Установить Item, который будет считаться открытым
    static void SetOpenedItem(Item *item);
    /// Установить текущий Item - именно к нему будут приходить события ручки и ESC
    static void SetCurrentItem(Item *item);
    /// Сброс текущего Item - текущего Item'a больше нет
    static void ResetCurrentItem();
    /// Возвращает текущий Item
    static Item* GetCurrentItem();
    /// Обнулить открытый итем - итем закрыт
    static void ResetOpenedItem();
    /// Возвращает позицию страницы на страницы. Только если страница принадлежит главному меню. Иначе -1
    static int GetPosition(const Page *page);
    /// Устанавливает для отрисовки дополнительноую страницу
    static void SetAdditionPage(Page *page);
    /// Синмает дополнительную страницу с отрисовки
    static void ResetAdditionPage();
    /// Указатель на главную страницу
    static Page *mainPage;
    ///\brief  Здесь хранится адрес элемента меню, соответствующего функциональной клавише [F1..F4], если она находится в нижнем положении, и 0, 
    /// если ни одна кнопка не нажата.
    static Item *pressedItem;

private:
    /// Обработать управляющее воздействие control
    static void ProcessContorl(Key &control);
    /// Обрабатывает состояние выходов. Возвращает true, если состояние вывода изменилось
    static bool ProcessOutputs(Key &control);
    /// Если какой-то итем раскрыт, то здесь его адрес
    static Item *openedItem;

    static Item *currentItem;
};
