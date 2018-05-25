#pragma once


class Console
{
public:
    static void AddString(char *string);

    static void Draw();

private:
    static const int MAX_STRINGS = 29;

    static const int MAX_SYMBOLS_IN_STRING = 64;
    /// Количество строк в консоли
    static int numStrings;
    /// Здесь хранятся строки консоли
    static char buffer[MAX_STRINGS][MAX_SYMBOLS_IN_STRING];
};
