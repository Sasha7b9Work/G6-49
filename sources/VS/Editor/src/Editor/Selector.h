#pragma once


/*

    "Выделитель" - всё, что связано с выделением

*/


class Selector
{
public:
    
    static void BeginSelect(int mouseX);
    
    static void MoveSelect(int mouseX);
    
    static void EndSelect(int mouseX, int mouseY);
    
    static void Draw();

    // Возвращает true, если курсор находится над границей выделенного участка
    static bool CursorOverBorder(int mouseX);
};
