#pragma once
#include "defines.h"
#include "Display/DisplayTypes.h"


typedef struct
{
    uchar width;
    uchar bytes[8];
} Symbol;


struct Font
{
    static int GetSize();
    static int GetLengthText(const char *text);
    static int GetHeightSymbol(char symbol);
    static int GetLengthSymbol(uchar symbol);
    
    int height;
    Symbol symbol[256];
};


extern const Font *font;
extern const Font *fonts[TypeFont_Number];

extern const uchar font5display[3080];
extern const uchar font8display[3080];
extern const uchar fontUGOdisplay[3080];
extern const uchar fontUGO2display[3080];
