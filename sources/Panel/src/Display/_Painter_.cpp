//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawBigChar(int eX, int eY, int size, char symbol)
{
    int8 width = (int8)font->symbol[symbol].width;
    int8 height = (int8)font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            SetPoint(x + i, y + j);
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Painter::BitInFontIsExist(int eChar, int numByte, int bit)
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawTextInRect(int x, int y, int width, const char *text)
{
    int xStart = x;
    int xEnd = xStart + width;

    while (*text != 0)
    {
        int length = GetLenghtSubString(text);
        if (length + x > xEnd)
        {
            x = xStart;
            y += Font::GetHeightSymbol(*text);
        }
        int numSymbols = 0;
        numSymbols = DrawSubString(x, y, text);
        text += numSymbols;
        x += length;
        x = DrawSpaces(x, y, text, &numSymbols);
        text += numSymbols;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawSpaces(int x, int y, const char *text, int *numSymbols)
{
    *numSymbols = 0;
    while (*text == ' ')
    {
        x = DrawChar(x, y, *text);
        text++;
        (*numSymbols)++;
    }
    return x;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawSubString(int x, int y, const char *text)
{
    int numSymbols = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        x = DrawChar(x, y, *text);
        numSymbols++;
        text++;
        x++;
    }
    return numSymbols;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::GetLenghtSubString(const char *text)
{
    int retValue = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        retValue += Font::GetLengthSymbol(*text);
        text++;
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawTextRelativelyRight(int xRight, int y, const char *text)
{
    int lenght = Font::GetLengthText(text);
    DrawText(xRight - lenght, y, text);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::Draw4SymbolsInRect(int x, int y, char eChar)
{
    for (char i = 0; i < 2; i++)
    {
        DrawChar(x + 8 * i, y, eChar + i);
        DrawChar(x + 8 * i, y + 8, eChar + i + 16);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawBigText(int eX, int eY, int size, const char *text)
{
    int numSymbols = (int)strlen(text);

    int x = eX;

    for (int i = 0; i < numSymbols; i++)
    {
        x = DrawBigChar(x, eY, size, text[i]);
        x += size;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawTextC(int x, int y, const char *text, Color col)
{
    SetColor(col);
    return DrawText(x, y, text);
}
