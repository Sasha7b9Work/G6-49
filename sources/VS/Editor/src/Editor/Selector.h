#pragma once


/*

    "Выделитель" - всё, что связано с выделением

*/


class Selector
{
public:
    
    static void BeginGrab(int mouseX);
    
    static void MoveBorder(int mouseX);
    
    static void EndGrab();
    
    static void Draw();

    // Возвращает true, если курсор находится над границей выделенного участка
    static bool CursorOverBorder(int mouseX);
};
