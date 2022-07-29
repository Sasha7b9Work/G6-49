// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


namespace Menu
{
    // Столько элементов меню помещается на дисплее одновременно
    static const int NUM_ITEMS_ON_DISPLAY = 4;

    void Init();

    void Update();

    void Draw();

    // Нарисовать заголовок
    int DrawTitle();

    void DrawPagesUGO(int x);

    // Возвращает true, если ручка управляет переклюичением подстраниц меню
    bool RegIsControlSubPages();

    // Возвращает адрес открытого элемента меню
    Item *GetOpenedItem();

    // Установить Item, который будет считаться открытым
    void SetOpenedItem(Item *item);

    // Установить текущий Item - именно к нему будут приходить события ручки и ESC
    void SetCurrentItem(Item *item);

    // Сброс текущего Item - текущего Item'a больше нет
    void ResetCurrentItem();

    // Возвращает текущий Item
    Item *GetCurrentItem();

    // Обнулить открытый итем - итем закрыт
    void ResetOpenedItem();

    // Возвращает позицию страницы на страницы. Только если страница принадлежит главному меню. Иначе -1
    int GetPosition(const Page *page);

    // Устанавливает для отрисовки дополнительноую страницу
    void SetAdditionPage(Page *page);

    // Синмает дополнительную страницу с отрисовки
    void ResetAdditionPage();

    // Здесь хранится адрес элемента меню, соответствующего функциональной клавише [F1..F4], если она находится в нижнем положении, и 0, если ни одна кнопка не нажата.
    extern Item *pressedItem;
};
