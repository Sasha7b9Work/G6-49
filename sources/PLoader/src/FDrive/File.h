#pragma once
#include "common/Messages.h"


class String;



class File
{
public:
    File();
    ~File();
    /// Открывает файл
    void Open(int num);
    /// Закрывает файл и освобождает память
    void Close();

    static bool Handler(SimpleMessage *msg);
};


struct FileRequests
{
    /// Считать строку из файла
    static void SendRequestForString(const String *name, int numString);
};
