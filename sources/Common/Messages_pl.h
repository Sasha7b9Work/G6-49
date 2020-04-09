#pragma once
#pragma once
#include "common/Messages.h"


namespace Message
{
    // Этим сообщением панель делает запрос на обновление
    class RequestUpgrade : public SimpleMessage
    {
    public:
        RequestUpgrade();
    };

    // Этим сообщением устройство сообщает размер проапгрейженой части.
    // mode : 0 - производится обновление device; 1 - производится обновление panel
    class PortionUpdate : public SimpleMessage
    {
    public:
        PortionUpdate(int mode, int portion);
    };

    // Этим сообщением device даёт панели разрешение на запуск
    class StartMainApplication : public SimpleMessage
    {
    public:
        StartMainApplication();
    };
}
