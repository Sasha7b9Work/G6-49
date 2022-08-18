// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Controls.h"


namespace Keyboard
{
    void Init();

    void Update();

    void InitInputs(const uint16 *sl, pchar portSL, int numSL, const uint16 *rl, pchar portRL, int numRL);
    
    // Возвращает true, если буфер пуст
    bool BufferIsEmpty();
    
    // Возвращает следующий орган управления, если таковой имеется
    Control GetNextControl();
    
    // Функция-заглушка. Для эмулятора, на котором нет физической клавиатуры и нужно её рисовать на экране
    void Draw();

    void AppendEvent(Key::E key, Action::E action);

    // Структура предназначена единственно для декодирования клавиатурных последовательностей
    namespace Decoder
    {
        // Возвращает true, если распознана последовательность, заданная массивом keys. Последнй элемент элемент массива должен быть Key::Count.
        // Последовательность распознана, если предыдущие поданные события совпадали с элементами массива в порядке очереди, а предпоследний равен key.
        // Размер массива не может превышать 20
        bool Decode(const Control controls[20], const Control &control);
    };
};
