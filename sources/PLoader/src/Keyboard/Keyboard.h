// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Controls.h"


namespace Keyboard
{
    void Init();

    // Возвращает true, если буфер пуст
    bool BufferIsEmpty();

    // Возвращает следующий орган управления, если таковой имеется
    Key GetNextControl();

    // Функция-заглушка. Для эмулятора, на котором нет физической клавиатуры и нужно её рисовать на экране
    void Draw();

    void AppendEvent(Key::E key, Key::Action action);

    // Структура предназначена единственно для декодирования клавиатурных последовательностей
    namespace Decoder
    {
        // Возвращает true, если распознана последовательность, заданная массивом keys. Последнй элемент элемент массива должен быть Key::Count.
        // Последовательность распознана, если предыдущие поданные события совпадали с элементами массива в порядке очереди, а предпоследний равен key.
        // Размер массива не может превышать 20
        bool Decode(const Key keys[20], const Key &key);
    };
};
