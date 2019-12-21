#include "defines.h"
#include "Display/DisplayTypes.h"
#include "Display/Text.h"
#include "Font.h"
#include "Utils/StringUtils.h"
#include "Settings/Settings.h"


#include "font7.inc"
#include "font5.inc"
#include "font8.inc"


static const Font *fonts[TypeFont::Count] = {&font5, &font7, &font8};
static const Font *font = &font7;


static TypeFont::E type = TypeFont::_5;


int Font::GetSize()
{
    return font->height;
}


void Font::ToggleCharacterSize()
{
    const Font *f = BIG_SYMBOLS ? (&font8) : (&font7);

    fonts[TypeFont::_7] = f;

    SetType(type);
}


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


int Font::GetHeightSymbol(char)
{
    return 9;
}


int Font::GetLengthSymbol(char symbol)
{
    return font->symbol[static_cast<uint8>(Text::IsUpperCase() ? SU::ToUpper(symbol) : symbol)].width + 1;
}


void Font::SetType(TypeFont::E typeFont)
{
    type = typeFont;
    font = fonts[type];
}


int8 Font::Width(char symbol)
{
    return Width(static_cast<uint8>(symbol));
}


int8 Font::Width(uint8 symbol)
{
    return static_cast<int8>(font->symbol[symbol].width);
}


int8 Font::Height()
{
    return static_cast<int8>(font->height);
}


bool Font::ByteNotEmpty(int s, int byte)
{
    static const uint8 *bytes = 0;
    static int prevChar = -1;
    if(s != prevChar)
    {
        prevChar = s;
        bytes = font->symbol[static_cast<uint8>(prevChar)].bytes;
    }
    return bytes[byte] != 0;
}


bool Font::BitIsExist(int s, int byte, int bit)
{
    static uint8 prevByte = 0;      /// \todo здесь точно статики нужны?
    static int prevChar = -1;
    static int prevNumByte = -1;
    if(prevNumByte != byte || prevChar != s)
    {
        prevByte = font->symbol[static_cast<uint8>(s)].bytes[byte];
        prevChar = s;
        prevNumByte = byte;
    }
    return prevByte & (1 << bit);
}
