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

    static uint8 *GetBuffer();
    
    // Экран
    static uint8 frontBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];
    
    // Задний буфер. В нём происходит отрисовка, и затем изображение копируется во frontBuffer
    static uint8 backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

    static void ShowWarning(String warning);
};
