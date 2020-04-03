#pragma once
#pragma once
#include "common/Messages.h"


namespace Message
{
    // Этим сообщением панель делает запрос на обновление
    class RequestUpdate : public SimpleMessage
    {
    public:
        RequestUpdate();
    };

    // Этим сообщением устройство сообщает размер проапгрейженой части
    class PortionUpdateDevice : public SimpleMessage
    {
    public:
        PortionUpdateDevice(int portion);
    };

    // Этим сообщением device даёт панели разрешение на запуск
    class StartMainApplication : public SimpleMessage
    {
    public:
        StartMainApplication();
    };
}
