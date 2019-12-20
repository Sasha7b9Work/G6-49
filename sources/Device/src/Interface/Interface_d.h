#pragma once
#include "Command.h"
#include "Message.h"
#include "Utils/Queue.h"



namespace Interface
{
    void Update();
    /// Возвращает ссылку на очередь сообщений, ожидающих отправки
    Queue &GetOutbox();

    void ResetFreqForSend();
};
