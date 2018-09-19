#include "defines.h"
#include "Display/DisplayTypes.h"
#include "Display/Text.h"
#include "Font.h"
#include "Utils/StringUtils.h"
#include "font7.inc"
#include "font5.inc"
#include "fontUGO.inc"
#include "fontUGO2.inc"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Font *fonts[Font::Type::Number] = {&font5, &font7, &fontUGO, &fontUGO2};
const Font *font = &font7;


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
void Font::Set(Type typeFont)
{
    font = fonts[typeFont];
}
