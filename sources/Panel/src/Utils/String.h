#pragma once
#include "Display/Colors.h"



class String
{
public:
    explicit String();
    /// Резервиврует для строки size символов
    explicit String(uint size);
             String(const String &);
    explicit String(char symbol);
    explicit String(const char *format, ...);
    ~String();

    void From(const char *format, ...);

    void From(const String &s);

    char *CString() const;
    /// Отобразить текст на экране в заданнх координатах
    int Draw(int x, int y, Color color = Color::NUMBER) const;
    /// Возвращает true, если строка ничего не содержит (не равно строке нулевой длины)
    bool IsEmpty() const { return buffer == 0; };
    /// Освободить память, занимаемую строкой
    void Release();

    String &operator=(const String &s)
    {
        From(s);
        return *this;
    }

    char &operator[] (int i) { return buffer[i]; };

private:

    bool Allocate(uint size);
    void Free();

    char *buffer;
};
