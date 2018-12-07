#pragma once
#include "Display/Colors.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class String
{
public:
    explicit String();
             String(const String &);
    explicit String(char symbol);
    explicit String(const char *format, ...);
    ~String();

    void From(const char *format, ...);

    char *CString() const;
    /// Отобразить текст на экране в заданнх координатах
    int Draw(int x, int y, Color color = Color::NUMBER) const;
    /// Возвращает true, если строка ничего не содержит (не равно строке нулевой длины)
    bool IsEmpty() const { return buffer == 0; };
    /// Освободить память, занимаемую строкой
    void Release();

private:

    bool Allocate(uint size);
    void Free();

    char *buffer;
};
