#pragma once


struct TypeConversionString
{
    enum E
    {
        None,           // Преобразование строки не производится
        FirstUpper      // Первый символ - в вернем регистре, остальные - в нижнем
    };
};


class String // -V690
{
public:
    explicit String();
             String(const String &);
    explicit String(char symbol);
    explicit String(const char *format, ...);
    ~String();

    void Set(TypeConversionString::E conv, const char *format, ...);

    char *c_str() const;

    void Free();

    void Append(const char *str);

    void Append(const char *str, uint numSymbols);

    void Append(char symbol);
    /// Удаляет numSymbols из начала строки
    void RemoveFromBegin(uint numSymbols);

    void RemoveFromEnd();

    uint Size() const;

    char &operator[](uint i);

private:

    bool Allocate(uint size);
    void Conversion(TypeConversionString::E conv);

    char *buffer;
};
