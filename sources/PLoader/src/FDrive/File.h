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
};
