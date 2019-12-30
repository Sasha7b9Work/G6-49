#pragma once



class IntValue
{
public:
    IntValue(const char *buffer, int posComma);
    /// Отнимает от числа 5
    void Sub5(char *bufferOut, int *posComma);
private:
    /// Целая часть
    int whole;
    /// Дробная часть - количество 1/1000 частей в числе
    int fract1000;

    int sign;

    int posComma;
    /// Преобразует в целоей число
    int ToWhole(pCHAR buffer);
    /// Преобразует в дробную часть
    int ToFract1000(pCHAR buffer);
    
    void ToString(char *buffer);
    /// Записывает целое число в буфер. Ширина поля для записи - numDigits
    void WholeToString(char *buffer, int numDigits);

    void Fract1000toString(char *buffer, int numDigits);
    /// Возвращает цифру из позиции pos дробной части
    int DigitFromFract1000(int pos);
};
