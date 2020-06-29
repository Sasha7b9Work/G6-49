#pragma once
#include "common/Messages.h"


class String;



class File
{
public:
    File();
    ~File();
    
    // Посылает запрос на содержимое файла с номером numberFile (точки сигнала)
    void RequestFromPicture(int numberFile);
    
    // Отображает содержимое
    void Draw(int x, int y);
    
    // Закрывает файл и освобождает память
    void Close();

    static bool Handler(SimpleMessage *msg);
    
    // Сохранить данные как данные формы
    static void SetDataToWave();
};


struct FileRequests
{
    // Считать строку из файла
    static void SendRequestForString(const String *name, int numString);
};
