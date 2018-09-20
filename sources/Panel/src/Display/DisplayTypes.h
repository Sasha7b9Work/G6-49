#pragma once
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240

#ifdef OPEN
#define BUFFER_WIDTH    1024            /* Ширина буфера для рисования */
#define BUFFER_HEIGHT   600             /* Высота буфера для рисования */
#else
#define BUFFER_WIDTH    SCREEN_WIDTH    /* Ширина буфера для рисования */
#define BUFFER_HEIGHT   SCREEN_HEIGHT   /* Высота буфера для рисования */
#endif

#define SIGNAL_HEIGHT   (FREQ_ENABLED ? ((SCREEN_HEIGHT - Page::Title::HEIGHT) / 2 - 10) :  ((SCREEN_HEIGHT - Page::Title::HEIGHT) / 2))
#define SIGNAL_WIDTH    ((SCREEN_WIDTH - Item::WIDTH) - 1)

#define MOI_HEIGHT_TITLE        19
#define MOSI_HEIGHT             14
