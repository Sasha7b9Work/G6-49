// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
    "Выделитель" - всё, что связано с выделением
*/


namespace Selector
{
    void BeginGrab(int mouseX);

    void MoveBorder(int mouseX);

    void EndGrab();

    void DrawRegion();

    void DrawCursors();

    // Возвращает true, если курсор находится над границей выделенного участка
    bool CursorOverBorder(int mouseX);

    bool IsEnabled();

    void Enable();

    void Disable();
};
