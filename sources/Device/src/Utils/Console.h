#pragma once
#include "Command.h"




class Console
{
public:
    static void AddString(char *buffer);
    //static void AddFormatString(char *format, ...);
    static void AddString(pString buffer);
    //static void AddFloat(float value);
    static void AddInt(int value);
    static bool ExistString();
    /// ¬озвращает передаваемую строку
    static char *GetString();
    /// ”дал€ет передаваемую строку из хранилища
    static void DeleteString();
};
