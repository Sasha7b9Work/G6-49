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
        enum E
        {
            _5,
            _7,
            _8,
            UGO,
            UGO2,
            Number,
            None
        } value;
        Type(E v) : value(v) {};
        Type(int v) : value((E)v) {};
        operator uint8() const { return (uint8)value; };
    };

    static int GetSize();
    static int GetLengthText(const char *text);
    static int GetHeightSymbol(char symbol);
    static int GetLengthSymbol(char symbol);
    static void SetType(Type typeFont);
    static Type GetType();
    static int GetWidth(char symbol);
    static bool ByteFontNotEmpty(int eChar, int byte);
    static bool BitInFontIsExist(int eChar, int numByte, int bit);
    
    int height;
    Symbol symbol[256];
};


#ifdef WIN32
#pragma warning(pop)
#endif
