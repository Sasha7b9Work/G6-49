#pragma once


struct Keyboard
{
    friend class CPU;

private:

    static void Init();

public:
    static void InitInputs(const uint16 *sl, const char *portSL, int numSL, const uint16 *rl, const char *portRL, int numRL);
    /// Возвращает true, если буфер пуст
    static bool BufferIsEmpty();
    /// Возвращает следующий орган управления, если таковой имеется
    static KeyEvent GetNextControl();
    /// Функция-заглушка. Для эмулятора, на котором нет физической клавиатуры и нужно её рисовать на экране
    static void Draw();

private:

    static void Update();
};
