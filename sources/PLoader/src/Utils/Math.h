// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define LIMITATION(var, min, max)           if(var < (min)) { (var) = (min); } else if(var > (max)) { var = (max); } else { var = var; };


namespace Math
{
    // ���������� ������������ �������� �� ���
    float MaxFloat(float val1, float val2, float val3);

    bool IsEquals(float x, float y);

    int DigitsInIntPart(float value);

    int DigitsInInt(int value);

    // \brief ��������� ����� � ��������� ������. numDigits - ������ ����� ������, �� ������� ������������ ����������.
    // ��������, RoundFloat(12.37137, 4) �������� �� 12.40)
    float RoundFloat(float value, int numDigits);

    // ���������� 10**pow.
    uint Pow10(int pow);

    // ���������� ������� ���������� ������ �� ������� value0 � value1
    template<class T> void Swap(T *value0, T *value1) { T temp = *value0; *value0 = *value1; *value1 = temp; }

    template<class T> int Sign(T x) { if(x > (T)(0)) { return 1; } if(x < (T)(0)) { return -1; } return 0; }

    template<class T> T Abs(T x) { if(x < (T)0) { return -x; }  return x; }

    template<class T> void CircleIncrease(T *value, T min, T max)
    {
        if(*value < max) { ++(*value); }
        else { *value = (T)min; }
    }

    template<class T> void CircleDecrease(T *value, T min, T max)
    {
        if(*value > min) { --(*value); }
        else { *value = (T)max; }
    }

    template<class T> void Limitation(T *value, T min, T max)
    {
        if(*value < min) { *value = min; }
        else if(*value > max) { *value = max; }
        else
        {
            // ������ ������ �� �����
        }
    }
};
