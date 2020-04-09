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

    // Этим сообщением устройство сообщает размер записанной части прошивки device
    class PortionUpgradeDevice : public SimpleMessage
    {
    public:
        PortionUpgradeDevice(int portion);
    };

    // Этим сообщением устройство сообщает размер записанной части прошивки panel и также передаёт данные
    class PortionUpgradePanel : public SimpleMessage
    {
    public:
        PortionUpgradePanel(int num, int portion, uint8 *data, int size);
    };

    // Этим сообщением device даёт панели разрешение на запуск
    class StartMainApplication : public SimpleMessage
    {
    public:
        StartMainApplication();
    };
}
