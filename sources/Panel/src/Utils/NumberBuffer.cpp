#include "defines.h"
#include "Display/Text.h"
#include "Utils/NumberBuffer.h"
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


void NumberBuffer::ProcessKey(const Key::E key)
{
    if (key == Key::RotateRight || key == Key::RotateLeft)
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
    else if(AllowableSymbol(key) && position < NumSymbols())                    // Символьная кнопка //-V2516
    {
        buffer[position++] = Key(key).ToChar(); //-V2563
    }
}


bool NumberBuffer::AllowableSymbol(const Key::E key) //-V2506
{
    if(Key(key).IsDigit() || (key == Key::Comma) || (key == Key::Minus))
    {
        return true;
    }

    return false;
}


void NumberBuffer::ProcessRegulator(const Key::E key) //-V2506
{
    if(NumSymbols() == 0)                           // Если буфер пуст -
    {
        return;                                     // просто выходим, ничего не делая
    }

    // Сохраняем старое значение, чтобы восстановить его в случае, если при изменении оно превысит максимальное
    char temp[32];
    std::memcpy(temp, buffer, static_cast<uint>(size));

    if (key == Key::RotateRight)
    {
        IncreaseDigit(PositionSymbolForChange());
    }
    else if(key == Key::RotateLeft) //-V2516
    {
        DecreaseDigit(PositionSymbolForChange());
    }

    // Восстанавливаем старое значение, если новое вышло за пределы
    if(max != 0 && ToUINT() > static_cast<uint>(max))
    {
        std::memcpy(buffer, temp, static_cast<uint>(size));
    }
}


int NumberBuffer::PositionSymbolForChange()
{
    return (NumSymbols() == position) ? (position - 1) : position;
}


bool NumberBuffer::IncreaseDigit(int pos) //-V2506
{
    if(buffer[pos] < '9') //-V2563
    {
        ++buffer[pos]; //-V2563
        return true;
    }
    else
    {
        if (pos > 0)
        {
            if (IncreaseDigit(pos - 1))
            {
                buffer[pos] = '0'; //-V2563
                return true;
            }
        }
        else 
        {
            if (position == NumSymbols() && All9())
            {
                buffer[0] = '1'; //-V2563
                position++;
                for (int i = 1; i < position; i++)
                {
                    buffer[i] = '0'; //-V2563
                }
                buffer[position + 1] = 0; //-V2563
                return true;
            }
        }
    }

    return false;
}


bool NumberBuffer::All9() //-V2506
{
    for(int i = 0; i < NumSymbols(); i++)
    {
        if(buffer[i] != '9') //-V2563
        {
            return false;
        }
    }
    return true;
}


bool NumberBuffer::DecreaseDigit(int pos) //-V2506
{
    if(buffer[pos] > '0') //-V2563
    {
        --buffer[pos]; //-V2563
        return true;
    }
    else if(pos > 0) //-V2516
    {
        if(DecreaseDigit(pos - 1))
        {
            buffer[pos] = '9'; //-V2563
            return true;
        }
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
            buffer[i] = 0; //-V2563
        }
    }
}


int NumberBuffer::PositionCursor()
{
    return position;
}


uint NumberBuffer::ToUINT() //-V2506
{
    uint result = static_cast<uint>(-1);
    if(!SU::String2UInt(buffer, &result))
    {
        return static_cast<uint>(-1);
    }

    return result;
}
