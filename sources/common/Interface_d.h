// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Command.h"
#include "common/Messages.h"
#include "Utils/Queue.h"


namespace DInterface
{
    void Update();

    // Возвращает ссылку на очередь сообщений, ожидающих отправки
    Queue &GetOutbox();

    void ResetFreqForSend();

    // Добавляет сообщение в очередь отправки. Если очередь заполнена, сообщение удаляется и функция возвращает false
    bool AddMessageForTransmit(SimpleMessage *message);
};
