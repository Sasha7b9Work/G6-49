// 2022/8/1 11:34:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/DisplayTypes.h"
#include "Display/Text.h"
#include "Keyboard/Controls.h"


#define BUFFER_WIDTH    Display::WIDTH    // Ширина буфера для рисования
#define BUFFER_HEIGHT   Display::HEIGHT   // Высота буфера для рисования


namespace Display
{
    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    void Init();

    void Update();

    void DrawScreenSplash();

    uint8 *GetBuffer();

    uint8 *GetRow(int row);

    // Экран
    extern uint8 frontBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

    // Задний буфер. В нём происходит отрисовка, и затем изображение копируется во frontBuffer
    extern uint8 backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

    namespace Warnings
    {
        void Clear();

        namespace Top
        {
            void Show(const String &ru, const String &en);
            void Clear();
        };

        namespace Center
        {
            void _Show(const String &ru, const String &en);
            void _Show(pchar ru, pchar en);
            void _Show2Strings(const String &ru1, const String &ru2, const String &en1, const String &en2);
        };
    };
};
