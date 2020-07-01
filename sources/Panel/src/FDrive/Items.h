#pragma once
#include "common/command.h"
#include "common/Messages.h"



class File;

class ListFiles
{
public:

    static void Init();

    // true означает, что идёт ожидание ответа от устройства
    static bool WaitAnswer()
    {
        return requestIsSend;
    }

    // Послать запрос на количество итемов
    static void SendRequest();

    // Обработка нажатия кнопки "Вверх"
    static void PressUp();

    // Обработка нажатия кнопки "Вниз"
    static void PressDown();

    // Возвращает количество файлов. если == -1, запрос не посылался
    static int NumberFiles();

    // Нарисовать списокфайлов
    static void Draw(int x, int y);

    // Возвращает номер текущего файла
    static int NumberCurrentFile();

    class Handler
    {
    public:
        static bool Processing(SimpleMessage *msg);
    };

private:

    // Запрос послан. Ожидается ответ
    static bool requestIsSend;
};
