#include "NumberBuffer.h"
#include "Utils/StringUtils.h"
#include <cstring>
#include <cstdlib>


char *NumberBuffer::buffer = 0;
int NumberBuffer::size = 0;
int NumberBuffer::position = 0;
int NumberBuffer::max = 0;



void NumberBuffer::Set(char *buf, int s, int p, int maxValue)
{
    buffer = buf;
    size = s;
    position = p;
    max = maxValue;
}


void NumberBuffer::ProcessKey(Key &key)
{
    if (key == Key::RegRight || key == Key::RegLeft)
    {
        ProcessRegulator(key);
    }
    else if(key == Key::Left)
    {
        if(position > 0)
        {
            --position;
        }
    }
    else if(key == Key::Right)
    {
        if(position < NumSymbols())
        {
            ++position;
        }
    }
    else if(AllowableSymbol(key) && position < NumSymbols())                    // —имвольна€ кнопка
    {
        buffer[position++] = key.ToChar();
    }
    else
    {
        // здесь ничего
    }
}


bool NumberBuffer::AllowableSymbol(Key &key)
{
    if(key.IsDigit() || key.Is(Key::Dot) || key.Is(Key::Minus))
    {
        return true;
    }

    return false;
}


void NumberBuffer::ProcessRegulator(Key &key)
{
    if(NumSymbols() == 0)                           // ≈сли буфер пуст -
    {
        return;                                     // просто выходим, ничего не дела€
    }

    // —охран€ем старое значение, чтобы восстановить его в случае, если при изменении оно превысит максимальное
    char temp[32];
    std::memcpy(temp, buffer, static_cast<uint>(size));

    if (key.Is(Key::RegRight))
    {
        IncreaseDigit(PositionSymbolForChange());
    }
    else if(key.Is(Key::RegLeft))
    {
        DecreaseDigit(PositionSymbolForChange());
    }
    else
    {
        // здесь ничего
    }

    // ¬осстанавливаем старое значение, если новое вышло за пределы
    if(max != 0 && ToUINT() > static_cast<uint>(max))
    {
        std::memcpy(buffer, temp, static_cast<uint>(size));
    }
}


int NumberBuffer::PositionSymbolForChange()
{
    return (NumSymbols() == position) ? (position - 1) : position;
}


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
    else
    {
        // здесь ничего
    }

    return false;
}


int NumberBuffer::NumSymbols()
{
    return static_cast<int>(size);
}


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


int NumberBuffer::PositionCursor()
{
    return position;
}


uint NumberBuffer::ToUINT()
{
    uint result = static_cast<uint>(-1);
    if(!SU::String2UInt(buffer, &result))
    {
        return static_cast<uint>(-1);
    }

    return result;
}
