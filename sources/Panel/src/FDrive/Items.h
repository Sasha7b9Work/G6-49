#pragma once
#include "command.h"
#include "Message.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Items
{
public:

    static void Init();
    /// true означает, что идёт ожидание ответа от устройства
    static bool WaitAnswer()
    {
        return requestIsSend;
    }
    /// Послать запрос на количество итемов
    static void SendRequest();
    /// Возвращает количество итемов. -1 - запрос не посылался
    static int NumberDirs()
    {
        return numDirs;
    }
    static int NumberFiles()
    {
        return numFiles;
    }

    static pString GetNameItem(int i);

    class Handler
    {
        friend class FDrive;
        static bool Processing(Message *msg);
    };

private:
    /// Количество каталогов в текущем каталоге
    static int numDirs;
    /// Количество файлов в текущем каталоге
    static int numFiles;
    /// Запрос послан. Ожидается ответ
    static bool requestIsSend;

};
