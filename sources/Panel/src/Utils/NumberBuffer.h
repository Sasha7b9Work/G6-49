// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Controls.h"


/*
    Агрегатирует функции для работы с символьным буфером, в котором хранится числовое значение.
    Например:
    буфер char buffer[10] = {'1', '5', '3', '0', '\0'}
    представляет число 1530.
    Функции позволяют обрабатывать нажатия кнопок и повороты ручки
*/

namespace NumberBuffer
{
    // \brief Задаёт первоначальные установки. size - размер буфера c учётом завершающего нуля, position - текущая позиция курсора, maxValue - 
    // максимальное значение, которое может храниться в буфере.
    void Set(char *buffer, int size, int position, int maxValue);

    // Обработка клавиши
    void ProcessKey(const Key::E key);

    // Обработка Backspace
    void PressBackspace();

    // Возвращает номер разряда, в котором находится курсор
    int PositionCursor();
};
