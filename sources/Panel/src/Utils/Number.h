#pragma once

/*
    Класс для работы с целочисленной арифметикой
*/

class Number
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
        operator uint8() const { return static_cast<uint8>(value); };
    };

    /// Размеры буферов, в которых хранятся целая и дробная части
    static const int SIZE_PART = 20;
    /// Установка буферов. В них должны быть занесены значения. Операции будут совершаться непосредственно над данными в этих буферах
    static void Set(char integer[SIZE_PART], char fract[SIZE_PART], char sign);
    /// Возвращает значение, представленное содержимым буферов
    static float GetValue();
    /// \brief Изменяет значение в позиции position на +1 или -1.
    /// При этом позиция -1 соответсвует первой позиции слева от точки (последняя цифра целого), а позиция +1 - первой позиции справа от точки
    /// (первая цифра дробного)
    static void ChangeDigit(int position, Step step);

    static char *integer;
    
    static char *fract;

    static char sign;

private:
    /// Возвращает значение, содержащееся в integer
    static uint64 GetInteger(pCHAR buffer);
    /// Изменение положительного числа
    static void ChangeInPositiveInteger(int position, Step step);

    static void ChangeInNegativeInteger(int position, Step step);
    /// Изменение отрицательного числа
    static void ChangeInPositiveFract(int position, Step step);

    static void ChangeInNegativeFract(int position, Step step);
    /// Возвращает true, если все символы слева от данной позиции - нули
    static bool AllLeftZeroInInteger(int position);
    /// Установить числу нулевое значение
    static void SetAllZeros();
};
