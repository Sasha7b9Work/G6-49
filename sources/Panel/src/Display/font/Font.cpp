#include "defines.h"
#include "Display/DisplayTypes.h"
#include "Display/Text.h"
#include "Font.h"
#include "Utils/StringUtils.h"
#include "Settings/Settings.h"


#include "font7.inc"
#include "font5.inc"
#include "font8.inc"



const Font *fonts[Font::Type::Count] = {&font5, &font7, &font8};
const Font *font = &font7;


static Font::Type type = Font::Type::_5;


int Font::GetSize()
{
    return font->height;
}


void Font::ToggleCharacterSize()
{
    const Font *f = BIG_SYMBOLS ? (&font8) : (&font7);

    fonts[Type::_7] = f;

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


void Font::SetType(Type typeFont)
{
    type = typeFont;
    font = fonts[type];
}
