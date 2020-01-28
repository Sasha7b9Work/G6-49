#pragma once


class String																// -V690
{
public:
             String(const String &);
    explicit String(char symbol = '\0');
    explicit String(const char *format, ...);
    ~String();

    char *CString() const;

private:

    bool Allocate(uint size);
    void Free();

    char *buffer;
};
