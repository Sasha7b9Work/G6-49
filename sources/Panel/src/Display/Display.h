#pragma once
#include "DisplayTypes.h"
#include "defines.h"
#include "Hardware/Controls.h"
#include "Settings/SettingsTypes.h"



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DISPLAY_SHOW_WARNING(warn)

#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240

#ifdef OPEN
#define BUFFER_WIDTH    1024            /* Ширина буфера для рисования */
#define BUFFER_HEIGHT   600             /* Высота буфера для рисования */
#else
#define BUFFER_WIDTH    SCREEN_WIDTH    /* Ширина буфера для рисования */
#define BUFFER_HEIGHT   SCREEN_HEIGHT   /* Высота буфера для рисования */
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Display
{
public:
    static void Init();

    static void Update();

    static uint8 *GetBuffer();

private:
    /// Экран
    static uint8 frontBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];
    /// Задний буфер. В нём происходит отрисовка, и затем изображение копируется во frontBuffer
    static uint8 backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];
};
