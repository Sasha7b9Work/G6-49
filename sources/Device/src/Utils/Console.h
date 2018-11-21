#pragma once
#include "Command.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Console
{
public:
    static bool ExistString();
    /// ¬озвращает передаваемую строку
    static char *GetString();
    /// ”дал€ет передаваемую строку из хранилища
    static void DeleteString();

    static void AddString(char *format, ...);

private:
    static void AddConstString(char *buffer);
    static void AddConstString(pString buffer);
};
