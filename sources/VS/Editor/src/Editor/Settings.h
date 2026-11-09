// (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


struct ModeButtonLeft
{
    enum E
    {
        EditPoints,     // Режим редактирования отдельных точек
        EditLines       // Режим редактирования интерполирующими прямыми
    };

    static E Get();

    static void Set(E v);
};
