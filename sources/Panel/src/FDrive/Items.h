// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/command.h"
#include "common/Messages.h"


class File;


namespace ListFiles
{
    void Init();

    // true означает, что идёт ожидание ответа от устройства
    bool WaitAnswer();

    // Послать запрос на количество итемов
    void SendRequest();

    // Обработка нажатия кнопки "Вверх"
    void PressUp();

    // Обработка нажатия кнопки "Вниз"
    void PressDown();

    // Возвращает количество файлов. если == -1, запрос не посылался
    int NumberFiles();

    // Нарисовать списокфайлов
    void Draw(int x, int y);

    // Возвращает номер текущего файла
    int NumberCurrentFile();

    struct Handler
    {
        static bool Processing(SimpleMessage *msg);
    };
};
