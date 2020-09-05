#pragma once


struct ModeButtonLeft
{
    enum E
    {
        EditPoints,     // Режим редактирования отдельных точек
        EditLines,      // Режим редактирования интерполирующими прямыми
        SelectZone      // Режим выделения зоны
    };

    static E Get();

    static void Set(E v);
};
