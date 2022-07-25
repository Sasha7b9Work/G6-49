// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Command.h"
#include "common/Messages.h"
#include "Utils/Queue.h"


struct DInterface
{
    static void Update();
    
    // Возвращает ссылку на очередь сообщений, ожидающих отправки
    static Queue &GetOutbox();

    static void ResetFreqForSend();
    
    // Добавляет сообщение в очередь отправки. Если очередь заполнена, сообщение удаляется и функция возвращает false
    static bool AddMessageForTransmit(SimpleMessage *message);
private:
    
    // Очередь сообщений, ожидающих отправки
    static Queue outbox;
    
    // Ненулевое значение означает, что его следует передать в панель как измеренное частотомером значение
    static uint freqForSend;
};
