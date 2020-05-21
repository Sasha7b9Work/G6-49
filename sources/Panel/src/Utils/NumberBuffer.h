#pragma once
#include "Keyboard/Controls.h"


/*
    Агрегатирует функции для работы с символьным буфером, в котором хранится числовое значение.
    Например:
    буфер char buffer[10] = {'1', '5', '3', '0', '\0'}
    представляет число 1530.
    Функции позволяют обрабатывать нажатия кнопок и повороты ручки
*/

class NumberBuffer
{
public:

    // \brief Задаёт первоначальные установки. size - размер буфера c учётом завершающего нуля, position - текущая позиция курсора, maxValue - 
    // максимальное значение, которое может храниться в буфере.
    static void Set(char *buffer, int size, int position, int maxValue);

    // Обработка клавиши
    static void ProcessKey(const Key::E key);

    // Обработка Backspace
    static void PressBackspace();

    // Возвращает номер разряда, в котором находится курсор
    static int PositionCursor();

private:
    static char *buffer;
    static int size;
    static int position;
    // Максимальное значение, которое может быть представлено в буфере. 0 означает, что ограничений на велечину значения нету
    static int max;

    // Возвращает количество символов в буфере (без учёта завершающего нуля).
    static int NumSymbols();

    // Обработка поворота ручки
    static void ProcessRegulator(const Key::E key);

    // Увеличить значение в заданном разряде. Возвращает true, если действие произошло
    static bool IncreaseDigit(int pos);

    // Уменьшить значение в заданном разряде. Возвращает true, если действие произошло
    static bool DecreaseDigit(int pos);

    // Возвращает позицию символа для изменения
    static int PositionSymbolForChange();

    // Является ли допустимым данный символ
    static bool AllowableSymbol(const Key::E key);

    // Возвращает числовое значение, представленное в буфере
    static uint ToUINT();

    // Возвращает true, если в числе все девятки
    static bool All9();
};
