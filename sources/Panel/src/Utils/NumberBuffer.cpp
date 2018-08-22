#include "NumberBuffer.h"
#include "Utils/StringUtils.h"
#include <string.h>
#include <stdlib.h>


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
    if (key == Control::REG_RIGHT || key == Control::REG_LEFT)
    {
        ProcessRegulator(key);
    }
    else if(key == Control::B_LEFT)
    {
        if(position > 0)
        {
            --position;
        }
    }
    else if(key == Control::B_RIGHT)
    {
        if(position < NumSymbols())
        {
            ++position;
        }
    }
    else if(AllowableSymbol(key) && position < NumSymbols())                    // Символьная кнопка
    {
        buffer[position++] = key.ToChar();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool NumberBuffer::AllowableSymbol(Control key)
{
    if(key.IsDigit() || key.Is(Control::B_Dot) || key.Is(Control::B_Minus))
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

    // Сохраняем старое значение, чтобы восстановить его в случае, если при изменении оно превысит максимальное
    char temp[32];
    memcpy(temp, buffer, (uint)size);

    if (key.Is(Control::REG_RIGHT))
    {
        IncreaseDigit(PositionSymbolForChange());
    }
    else if(key.Is(Control::REG_LEFT))
    {
        DecreaseDigit(PositionSymbolForChange());
    }

    // Восстанавливаем старое значение, если новое вышло за пределы
    if(max != 0 && ToUINT() > (uint)max)
    {
        memcpy(buffer, temp, (uint)size);
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
    else
    {
        if (pos > 0)
        {
            if (IncreaseDigit(pos - 1))
            {
                buffer[pos] = '0';
                return true;
            }
        }
        else 
        {
            if (position == NumSymbols() && All9())
            {
                buffer[0] = '1';
                position++;
                for (int i = 1; i < position; i++)
                {
                    buffer[i] = '0';
                }
                buffer[position + 1] = 0;
                return true;
            }
        }
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool NumberBuffer::All9()
{
    for(int i = 0; i < NumSymbols(); i++)
    {
        if(buffer[i] != '9')
        {
            return false;
        }
    }
    return true;
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
    return (int)size;
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint NumberBuffer::ToUINT()
{
    uint result = (uint)-1;
    if(!String2UInt(buffer, &result))
    {
        return (uint)-1;
    }

    return result;
}
