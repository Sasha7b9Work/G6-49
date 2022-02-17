#pragma once


struct Bit
{
    template<typename T>
    static bool Get(T value, int bit)
    {
        return ((value >> bit) & 0x01) != 0;
    }

    template<typename T>
    static void Set(T &value, int bit)
    {
        value |= 1 << bit;
    }

    template<typename T>
    static void Clear(T &value, int bit)
    {
        value &= ~(1 << bit);
    }
};


struct Math
{
    // Вычисление 10**pow.
    static uint Pow10(int pow);

    template<class T> static T Limitation(T *value, T min, T max)
    {
        if(*value < min)
        {
            *value = min;
        }
        else if(*value > max)
        {
            *value = max;
        }

        return *value;
    }

    template<class T> static int Sign(T x) { if(x > (T)(0)) { return 1; } if(x < (T)(0)) { return -1; } return 0; }
};
