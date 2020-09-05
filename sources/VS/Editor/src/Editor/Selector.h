#pragma once


/*

    "Выделитель" - всё, что связано с выделением

*/


class Selector
{
public:
    static void BeginSelect(int mouseX, int mouseY);
    static void MoveSelect(int mouseX, int mouseY);
    static void EndSelect(int mouseX, int mouseY);
private:
};
