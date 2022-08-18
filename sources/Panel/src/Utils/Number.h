// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

/*
    Класс для работы с целочисленной арифметикой
*/

namespace Number
{
    struct Step
    {
        enum E
        {
            Plus,
            Minus
        } value;
        Step(E v) : value(v) {};
        operator uint8() const { return (uint8)value; };
    };

    // Размеры буферов, в которых хранятся целая и дробная части
    static const int SIZE_PART = 20;

    // Установка буферов. В них должны быть занесены значения. Операции будут совершаться непосредственно над данными в этих буферах
    void Set(char integer[SIZE_PART], char fract[SIZE_PART], char sign);

    // Возвращает значение, представленное содержимым буферов
    float GetValue();

    // \brief Изменяет значение в позиции position на +1 или -1.
    // При этом позиция -1 соответсвует первой позиции слева от точки (последняя цифра целого), а позиция +1 - первой позиции справа от точки
    // (первая цифра дробного)
    void ChangeDigit(int position, Step step);

    extern char *integer;
    
    extern char *fract;

    extern char sign;
};
