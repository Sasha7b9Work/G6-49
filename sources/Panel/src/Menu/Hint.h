// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


class Hint
{
public:
    
    // Отрисовка подсказки
    static void Draw();
    
    // Орган упралвения. Возвращает false, если обработка не произошла
    static bool ProcessControl(const Control control);

private:
    
    // \brief Отрисовывает подсказки с пояснениями
    // start, end - начальный и конечный пункты choice для выводв
    // calculate - если true, то отрисовка не производится - только расчёт значений
    static int DrawDetailedHint(const Choice *choice, int x, int y, int width, int start, int end, bool calculate = false);
    
    // Возвращает число страниц в подсказке
    static int NumPagesInHint(const Choice *choice, int x, int y, int width);
    
    // Рассчитывает данные, необходимые для разбивки подсказки на страницы
    static void Calcualte(const Choice *choice, int x, int y, int width);
    
    // Рисует УГО страниц в многостраничной подсказке
    static void DrawPagesUGO(const int xRight, const int y);
    
    // Если true - нужно выводить подсказку на экран
    static bool show;
    
    // Контрол, для которого нужно выводить подсказку
    static const Item *item;
    
    // Орган управления, для которого нужно выводить подсказку
    static Key::E key;
    
    // Количество страниц в подсказке
    static int numPages;
    
    // Текущая отрисовываемая страница
    static int currentPage;
    
    // С какого пункта начинается вторая страница
    static int firstItemOnSecondPage;
    
    // Установленное в true значение означает, что Item был только что назначен и требуется разбивка на страницы
    static bool needCalculate;
};
