// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Text.h"
#include "Utils/NumberBuffer.h"
#include "Utils/StringUtils.h"
#include <cstring>
#include <cstdlib>


namespace NumberBuffer
{
    static char *buffer = nullptr;
    static int size = 0;
    static int position = 0;

    // ������������ ��������, ������� ����� ���� ������������ � ������. 0 ��������, ��� ����������� �� �������� �������� ����
    static int max = 0;

    // ���������� ���������� �������� � ������ (��� ����� ������������ ����).
    static int NumSymbols();

    // ��������� �������� �����
    static void ProcessRegulator(const Key::E key);

    // ��������� �������� � �������� �������. ���������� true, ���� �������� ���������
    static bool IncreaseDigit(int pos);

    // ��������� �������� � �������� �������. ���������� true, ���� �������� ���������
    static bool DecreaseDigit(int pos);

    // ���������� ������� ������� ��� ���������
    static int PositionSymbolForChange();

    // �������� �� ���������� ������ ������
    static bool AllowableSymbol(const Key::E key);

    // ���������� �������� ��������, �������������� � ������
    static uint ToUINT();

    // ���������� true, ���� � ����� ��� �������
    static bool All9();
}


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
    else if(AllowableSymbol(key) && position < NumSymbols())                    // ���������� ������
    {
        buffer[position++] = Key(key).ToChar();
    }
}


bool NumberBuffer::AllowableSymbol(const Key::E key)
{
    if(Key(key).IsDigit() || (key == Key::Comma) || (key == Key::Minus))
    {
        return true;
    }

    return false;
}


void NumberBuffer::ProcessRegulator(const Key::E key)
{
    if(NumSymbols() == 0)                           // ���� ����� ���� -
    {
        return;                                     // ������ �������, ������ �� �����
    }

    // ��������� ������ ��������, ����� ������������ ��� � ������, ���� ��� ��������� ��� �������� ������������
    char temp[32];
    std::memcpy(temp, buffer, (uint)size);

    if (key == Key::RotateRight)
    {
        IncreaseDigit(PositionSymbolForChange());
    }
    else if(key == Key::RotateLeft)
    {
        DecreaseDigit(PositionSymbolForChange());
    }

    // ��������������� ������ ��������, ���� ����� ����� �� �������
    if(max != 0 && ToUINT() > (uint)max)
    {
        std::memcpy(buffer, temp, (uint)size);
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

    return false;
}


int NumberBuffer::NumSymbols()
{
    return (int)size;
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
    uint result = (uint)(-1);
    if(!SU::String2UInt(buffer, &result))
    {
        return (uint)(-1);
    }

    return result;
}
