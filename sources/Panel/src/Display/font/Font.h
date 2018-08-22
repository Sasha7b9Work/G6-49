#pragma once
#include "defines.h"
#include "Display/DisplayTypes.h"


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


struct Symbol
{
    uchar width;
    uchar bytes[8];
};


struct Font
{
    struct Type
    {
        enum
        {
            _5,
            _8,
            _UGO,
            _UGO2,
            Number,
            None
        };
        uint8 value;
        Type(uint8 v) : value(v) {};
        operator uint8() const { return value; };
    };

    static int GetSize();
    static int GetLengthText(const char *text);
    static int GetHeightSymbol(char symbol);
    static int GetLengthSymbol(char symbol);
    
    int height;
    Symbol symbol[256];
};


extern const Font *font;
extern const Font *fonts[Font::Type::Number];

extern const uchar font5display[3080];
extern const uchar font8display[3080];
extern const uchar fontUGOdisplay[3080];
extern const uchar fontUGO2display[3080];


#ifdef WIN32
#pragma warning(pop)
#endif
