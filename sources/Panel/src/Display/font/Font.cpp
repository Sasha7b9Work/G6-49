#include "defines.h"
#include "Display/DisplayTypes.h"
#include "Display/Text.h"
#include "Display/Font/AdvancedFont.h"
#include "Display/Font/Font.h"
#include "Settings/Settings.h"
#include "Utils/Stack.h"
#include "Utils/StringUtils.h"


#include "font7.inc"
#include "font5.inc"
#include "font8.inc"


static const Font *fonts[TypeFont::Count] = {&font5, &font7, &font8};
static const Font *font = &font7;


TypeFont::E Font::type = TypeFont::_5;


static Stack<TypeFont::E> stackFonts(10);       // Здесь хранятся сменяемые шрифты для пследующего восстановления


int Font::GetSize()
{
    return font->height;
}


void Font::ToggleCharacterSize()
{
    const Font *f = set.bigSymbols ? (&font8) : (&font7);

    fonts[TypeFont::_7] = f;

    Set(type);
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
    if(font)
    {
        return font->symbol[static_cast<uint8>(Text::IsUpperCase() ? static_cast<uint8>(SU::ToUpper(symbol)) : static_cast<uint8>(symbol))].width + 1;
    }
    
    return AdvancedFont::GetWidth(static_cast<uint8>(symbol));
}


void Font::Set(TypeFont::E typeFont)
{
    type = typeFont;

    if(type > TypeFont::_8)
    {
        AdvancedFont::Set(type);
        font = nullptr;
    }
    else
    {
        font = fonts[type];
    }
}


void Font::StoreAndSet(TypeFont::E typeFont)
{
    Store();
    Set(typeFont);
}


int8 Font::Width(char symbol)
{
    return Width(static_cast<uint8>(symbol));
}


int8 Font::Width(uint8 symbol)
{
    if(IsAdvanced())
    {
        return static_cast<int8>(AdvancedFont::GetWidth(symbol));
    }

    return static_cast<int8>(font->symbol[symbol].width);
}


int8 Font::Height()
{
    if(IsAdvanced())
    {
        return static_cast<int8>(AdvancedFont::GetHeight());
    }

    return static_cast<int8>(font->height);
}


bool Font::RowNotEmpty(int s, int row)
{
    if(IsAdvanced())
    {
        return AdvancedFont::RowNotEmpty(static_cast<uint8>(s), row);
    }

    static const uint8 *bytes = 0;
    static int prevChar = -1;
    if(s != prevChar)
    {
        prevChar = s;
        bytes = font->symbol[static_cast<uint8>(prevChar)].bytes;
    }
    return bytes[row] != 0;
}


bool Font::BitIsExist(int s, int row, int bit)
{
    if(IsAdvanced())
    {
        return AdvancedFont::BitIsExist(static_cast<uint8>(s), row, bit);
    }

    static uint8 prevByte = 0;      // \todo здесь точно статики нужны?
    static int prevChar = -1;
    static int prevNumByte = -1;
    if(prevNumByte != row || prevChar != s)
    {
        prevByte = font->symbol[static_cast<uint8>(s)].bytes[row];
        prevChar = s;
        prevNumByte = row;
    }
    return prevByte & (1 << bit);
}


void Font::Store()
{
    stackFonts.Push(type);
}


void Font::Restore()
{
    Set(stackFonts.Pop());
}


bool Font::IsAdvanced()
{
    return (type > TypeFont::_8);
}
