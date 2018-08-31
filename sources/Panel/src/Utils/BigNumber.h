#pragma once
#include "defines.h"
#include "Utils/Math.h"


class BigNumber
{
public:

    struct Step
    {
        enum E
        {
            Plus,
            Minus
        } value;
        Step(E v) : value(v) {};
        bool IsPlus() { return value == Plus; };
    };

    static const int SIZE = 10;

    static void Set(char integer[SIZE], char fract[SIZE], char sign);

    static void ChangeDigit(int position, Step step);

    static float GetValue();

    static void Get(char integer[SIZE], char fract[SIZE], char *sign);

private:

    struct Number
    {
        /// Число, в которое была переведена строка символов, его представляющая
        uint value;
        /// Количество символов в исходной строке
        int numSymbols;

        Number() : value(0), numSymbols(0) {};

        void Clear() { value = 0; numSymbols = 0; };

        void AddValue(char digit, int pow) { value += (digit - 0x30) * pow; numSymbols++; };
        /// Возвращает максимально возможное число, которое можно хранить
        uint Max() { return Pow10(numSymbols) - 1; };
    };

    /// Здесь число из строки целого
    static Number integer;
    /// Здесь число из строки дробного
    static Number fract;

    static char sign;

    static void FillValue(char buffer[SIZE], Number &number);
    /// Изменяет значение целой части. Позиция отсчитывается с конца. Последний символ - нулевой.
    static void ChangeInteger(int position, Step step);
    /// Изменяет значение дробной части. Позиция отсчитывается с конца. Последний символ - нулевой.
    static void ChangeFract(int position, Step step);

    static void ChangeIntegerPositive(int position, Step step);

    static void ChangeIntegerNegative(int position, Step step);

    static void ChangeFractPositive(int position, Step step);

    static void ChangeFractNegative(int position, Step step);

    static void ChangeIntegerPositivePlus(int position);

    static void ChangeIntegerPositiveMinus(int position);

    static void ChangeIntegerNegativePlus(int position);

    static void ChangeIntegerNegativeMinus(int position);

    static void ChangeFractPositivePlus(int position);

    static void ChangeFractPositiveMinus(int position);

    static void ChangeFractNegativePlus(int position);

    static void ChangeFractNegativeMinus(int position);
};
