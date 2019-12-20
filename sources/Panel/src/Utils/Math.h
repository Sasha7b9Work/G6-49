#pragma once
#include "defines.h"


#define LIMITATION(var, min, max)           if(var < (min)) { (var) = (min); } else if(var > (max)) { var = (max); } else { var = var; };


///// Возвращает максимальное значение из трёх
float MaxFloat(float val1, float val2, float val3);

bool IsEquals(float x, float y);


struct Math
{

    static int DigitsInIntPart(float value);
    /// \brief Округляет число с плавающей точкой. numDigits - полное число знаков, по которым производится округление.
    /// Например, RoundFloat(12.37137, 4) округлит до 12.40)
    static float RoundFloat(float value, int numDigits);
};


/// Вычисление 10**pow.
uint Pow10(int pow);

/// Обменивает местами содержимое памяти по адресам value0 и value1
template<class T> void Swap(T *value0, T *value1) { T temp = *value0; *value0 = *value1; *value1 = temp; }

/// Размещает переменные value0 и value1 в порядке возрастания
template<class T> void Sort(T *value0, T *value1) { if (*value1 < *value0) { Swap<T>(value0, value1); } }

template<class T> int Sign(T x) { if (x > (T)(0)) { return 1; } if (x < (T)(0)) { return -1; } return 0; }

template<class T> T Abs(T x) { if (x < (T)0)   { return -x; }  return x; }

template<class T> T Min(T x1, T x2) { return x1 < x2 ? x1 : x2; }

template<class T> T Max(T x1, T x2) { return x1 > x2 ? x1 : x2; }

template<class T> void CircleIncrease(T *value, T min, T max)
{
    if (*value < max) { ++(*value); }
    else              { *value = (T)min; }
}

template<class T> void CircleDecrease(T *value, T min, T max)
{
    if (*value > min) { --(*value); }
    else              { *value = (T)max; }
}
/// Инкрементировать *value, но не больше, чем max
template<class T> static void LimitationIncrease(T *value, T max) { if ((*value) < max) { ++(*value); } }
/// Декрементировать *value, но не меньше, чем min
template<class T> static void LimitationDecrease(T *value, T min) { if (*value > min) { --(*value); } }

template<class T> static void Limitation(T *value, T min, T max)
{
    if (*value < min)      { *value = min; }
    else if (*value > max) { *value = max; }
    else
    {
        // ничего делать не нужно
    }
}

template<class T> static T LimitationRet(T value, T min, T max)
{
    if (value < min) { return min; }
    if (value > max) { return max; }
    return value;
}
/// Прибавить к *value term и ограничить, если результат выходит за границы [min, max]
template<class T> static void AddtionThisLimitation(T *value, int term, T min, T max)
{
    if (term > 0){
        if (max - term >= *value) { *value += (T)term; }
        else                      { *value = max; } }
    else         {
        if (min - term <= *value) { *value += (T)term; }
        else                      { *value = min; } }
}
