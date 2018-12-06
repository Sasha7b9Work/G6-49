#pragma once
#include "command.h"
#include "Message.h"
#include "Utils/String.h"


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
    static int NumberDirs();
    /// Обработка нажатия кнопки "Вверх"
    static void PressUp();
    /// Обработка нажатия кнопки "Вниз"
    static void PressDown();
    /// Нарисовать списокфайлов
    static void DrawItems(int x, int y);

    class Handler
    {
        friend class FDrive;
        static bool Processing(Message *msg);
    };

private:
    /// Запрос послан. Ожидается ответ
    static bool requestIsSend;
};
