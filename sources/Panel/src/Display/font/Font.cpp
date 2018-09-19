#include "defines.h"
#include "Display/DisplayTypes.h"
#include "Display/Text.h"
#include "Font.h"
#include "Utils/StringUtils.h"
#include "font7.inc"
#include "font5.inc"
#include "font8.inc"
#include "fontUGO.inc"
#include "fontUGO2.inc"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Font *fonts[Font::Type::Number] = {&font5, &font7, &font8, &fontUGO, &fontUGO2};
const Font *font = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Font::GetSize()
{
    return font->height;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Font::GetLengthText(const char *text)
{
    int retValue = 0;
    while (*text)
    {
        retValue += GetLengthSymbol(*text);
        text++;
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Font::GetHeightSymbol(char)
{
    return 9;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Font::GetLengthSymbol(char symbol)
{
    return font->symbol[Text::IsUpperCase() ? SU::ToUpper(symbol) : symbol].width + 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Font::SetType(Type typeFont)
{
    font = fonts[typeFont];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Font::Type Font::GetType()
{
    for(int i = 0; i < Type::Number; i++)
    {
        if(font == fonts[i])
        {
            return (Type)i;
        }
    }
    return Type::Number;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Font::GetWidth(char symbol)
{
    return font->symbol[symbol].width;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Font::ByteFontNotEmpty(int eChar, int byte)
{
    static const uint8 *bytes = 0;
    static int prevChar = -1;
    if (eChar != prevChar)
    {
        prevChar = eChar;
        bytes = font->symbol[prevChar].bytes;
    }
    return bytes[byte];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Font::BitInFontIsExist(int eChar, int numByte, int bit)
{
    static uint8 prevByte = 0;      /// \todo здесь точно статики нужны?
    static int prevChar = -1;
    static int prevNumByte = -1;
    if (prevNumByte != numByte || prevChar != eChar)
    {
        prevByte = font->symbol[eChar].bytes[numByte];
        prevChar = eChar;
        prevNumByte = numByte;
    }
    return prevByte & (1 << bit);
}
