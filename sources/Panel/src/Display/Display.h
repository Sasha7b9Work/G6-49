#pragma once
#include "DisplayTypes.h"
#include "defines.h"
#include "Hardware/Controls.h"
#include "Settings/SettingsTypes.h"




#define DISPLAY_SHOW_WARNING(warn)

#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240

#define BUFFER_WIDTH    SCREEN_WIDTH    /* Ширина буфера для рисования */
#define BUFFER_HEIGHT   SCREEN_HEIGHT   /* Высота буфера для рисования */


namespace Display
{
    void Init();

    void Update();

    uint8 *GetBuffer();
    /// Экран
    extern uint8 frontBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];
    /// Задний буфер. В нём происходит отрисовка, и затем изображение копируется во frontBuffer
    extern uint8 backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];
};
