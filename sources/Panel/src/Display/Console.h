#pragma once


class Console
{
public:
    static void AddString(const char *string);

    static void Draw();

private:
    static const int MAX_STRINGS = 10;

    static const int MAX_SYMBOLS_IN_STRING = 64;
    /// Количество строк в консоли
    static int numStrings;
    /// Здесь хранятся строки консоли
    static char buffer[MAX_STRINGS][MAX_SYMBOLS_IN_STRING];
};
