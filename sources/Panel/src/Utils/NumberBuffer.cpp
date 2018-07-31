#include "NumberBuffer.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char *NumberBuffer::buffer = 0;
int NumberBuffer::size = 0;
int NumberBuffer::position = 0;
int NumberBuffer::max = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NumberBuffer::Set(char *buf, int s, int p, int maxValue)
{
    buffer = buf;
    size = s;
    position = p;
    max = maxValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void NumberBuffer::ProcessKey(Control key)
{
    if (key == REG_RIGHT || key == REG_LEFT)
    {
        ProcessRegulator(key);
    }
    else if(key == B_LEFT)
    {
        if(position > 0)
        {
            --position;
        }
    }
    else if(key == B_RIGHT)
    {
        if(position < NumSymbols())
        {
            ++position;
        }
    }
    else if(AllowableSymbol(key))                    // Символьная кнопка
    {
        buffer[position++] = KeyToChar(key);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool NumberBuffer::AllowableSymbol(Control key)
{
    if(KeyIsDigit(key) || key == B_Dot || key == B_Minus)
    {
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void NumberBuffer::ProcessRegulator(Control key)
{
    if(NumSymbols() == 0)                           // Если буфер пуст -
    {
        return;                                     // просто выходим, ничего не делая
    }

    if (key == REG_RIGHT)
    {
        IncreaseDigit(PositionSymbolForChange());
    }
    else if(key == REG_LEFT)
    {
        DecreaseDigit(PositionSymbolForChange());
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int NumberBuffer::PositionSymbolForChange()
{
    return (NumSymbols() == position) ? (position - 1) : position;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool NumberBuffer::IncreaseDigit(int pos)
{
    if(buffer[pos] < '9')
    {
        ++buffer[pos];
        return true;
    }
    else if(pos > 0)
    {
        if(IncreaseDigit(pos - 1))
        {
            buffer[pos] = '0';
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool NumberBuffer::DecreaseDigit(int pos)
{
    if(buffer[pos] > '0')
    {
        --buffer[pos];
        return true;
    }
    else if(pos > 0)
    {
        if(DecreaseDigit(pos - 1))
        {
            buffer[pos] = '9';
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int NumberBuffer::NumSymbols()
{
    return (int)strlen(buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void NumberBuffer::PressBackspace()
{
    if (position > 0)
    {
        position--;
        for (int i = position; i < size; i++)
        {
            buffer[i] = 0;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int NumberBuffer::PositionCursor()
{
    return position;
}
