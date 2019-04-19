#pragma once
#include "Command.h"
#include "Message.h"
#include "Utils/Queue.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Interface
{
    friend class SimpleMessage;
    friend class Handlers;

public:

    static void Update();
    
private:
    /// Добавляет сообщение в очередь отправки. Если очередь заполнена, сообщение удаляется и функция возвращает false
    static bool AddMessageForTransmit(SimpleMessage *message);
    /// Очередь сообщений, ожидающих отправки
    static Queue messages;
    /// Ненулевое значение означает, что его следует передать в панель как измеренное частотомером значение
    static uint freqForSend;
};
