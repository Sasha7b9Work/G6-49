#pragma once


#define LIMITATION(var, min, max)           if(var < (min)) { (var) = (min); } else if(var > (max)) { var = (max); } else { var = var; };


struct Math
{
    // ���������� ������������ �������� �� ���
    static float MaxFloat(float val1, float val2, float val3);

    static bool IsEquals(float x, float y);

    static int DigitsInIntPart(float value);

    static int DigitsInInt(int value);
    // \brief ��������� ����� � ��������� ������. numDigits - ������ ����� ������, �� ������� ������������ ����������.
    // ��������, RoundFloat(12.37137, 4) �������� �� 12.40)
    static float RoundFloat(float value, int numDigits);
    // ���������� 10**pow.
    static uint Pow10(int pow);

    // ���������� ������� ���������� ������ �� ������� value0 � value1
    template<class T> static void Swap(T *value0, T *value1) { T temp = *value0; *value0 = *value1; *value1 = temp; }

    template<class T> static int Sign(T x) { if(x > (T)(0)) { return 1; } if(x < (T)(0)) { return -1; } return 0; }

    template<class T> static T Abs(T x) { if(x < (T)0) { return -x; }  return x; }

    template<class T> static void CircleIncrease(T *value, T min, T max)
    {
        if(*value < max) { ++(*value); }
        else { *value = (T)min; }
    }

    template<class T> static void CircleDecrease(T *value, T min, T max)
    {
        if(*value > min) { --(*value); }
        else { *value = (T)max; }
    }

    template<class T> static void Limitation(T *value, T min, T max)
    {
        if(*value < min) { *value = min; }
        else if(*value > max) { *value = max; }
        else
        {
            // ������ ������ �� �����
        }
    }
};
