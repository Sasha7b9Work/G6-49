#pragma once
#include "Keyboard/Controls.h"


struct Keyboard
{
    friend class CPU;

public:
    static void InitInputs(const uint16 *sl, const char *portSL, int numSL, const uint16 *rl, const char *portRL, int numRL);
    /// Возвращает true, если буфер пуст
    static bool BufferIsEmpty();
    /// Возвращает следующий орган управления, если таковой имеется
    static Key GetNextControl();
    /// Функция-заглушка. Для эмулятора, на котором нет физической клавиатуры и нужно её рисовать на экране
    static void Draw();

    static void AppendEvent(Key::E key, Key::Action action);

    // Структура предназначена единственно для декодирования клавиатурных последовательностей
    struct Decoder
    {
        // Возвращает true, если распознана последовательность, заданная массивом keys. Последнй элемент элемент массива должен быть Key::Count.
        // Последовательность распознана, если предыдущие поданные события совпадали с элементами массива в порядке очереди, а предпоследний равен key.
        // Размер массива не может превышать 20
        static bool Decode(const Key keys[20], const Key &key);
    };

private:

    static void Init();

    static void Update();
};
