#pragma once
#include "Display/DisplayTypes.h"
#include "Display/Text.h"
#include "Keyboard/Controls.h"


#define BUFFER_WIDTH    Display::WIDTH    // Ширина буфера для рисования
#define BUFFER_HEIGHT   Display::HEIGHT   // Высота буфера для рисования


struct Display
{
    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    static void Init();

    static void Update();

    static void DrawScreenSplash();

    static uint8 *GetBuffer();

    static uint8 *GetRow(int row);
    
    static void ShowWarning(String warning);
    static void ShowWarning(const char *warning);

    static void ClearWarnings();

    // Экран
    static uint8 frontBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

    // Задний буфер. В нём происходит отрисовка, и затем изображение копируется во frontBuffer
    static uint8 backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];
};
