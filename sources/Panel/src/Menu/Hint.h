#pragma once
#include "Menu/MenuItems.h"
#include "Hardware/Controls.h"


class Hint
{
public:
    /// ќтрисовка подсказки
    static void Draw();
    /// ќрган упралвени€. ¬озвращает false, если обработка не произошла
    static bool ProcessControl(StructControl strCtrl);

private:
    /// \brief ќтрисовывает подсказки с по€снени€ми
    /// start, end - начальный и конечный пункты choice дл€ выводв
    /// calculate - если true, то отрисовка не производитс€ - только расчЄт значений
    static int DrawDetailedHint(Choice *choice, int x, int y, int width, int start, int end, bool calculate = false);
    /// ¬озвращает число страниц в подсказке
    static int NumPagesInHint(Choice *choice, int x, int y, int width);
    /// –ассчитывает данные, необходимые дл€ разбивки подсказки на страницы
    static void Calcualte(Choice *choice, int x, int y, int width);
    /// ≈сли true - нужно выводить подсказку на экран
    static bool show;
    ///  онтрол, дл€ которого нужно выводить подсказку
    static const Item *item;
    /// ќрган управлени€, дл€ которого нужно выводить подсказку
    static Control control;
    ///  оличество страниц в подсказке
    static int numPages;
    /// “екуща€ отрисовываема€ страница
    static int currentPage;
    /// — какого пункта начинаетс€ втора€ страница
    static int firstItemOnSecondPage;
    /// ”становленное в true значение означает, что Item был только что назначен и требуетс€ разбивка на страницы
    static bool needCalculate;
};
