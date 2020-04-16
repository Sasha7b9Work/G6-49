#pragma once
#pragma once
#include "common/Messages.h"


#define SIZE_CHUNK        512            /* Размер элементарной порции данных */


namespace Message
{
    // Этим сообщением device даёт панели разрешение на запуск
    class StartMainApplication : public SimpleMessage
    {
    public:
        StartMainApplication();
    };

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

    // Это сообщение DLoader посылает в PLoader, если обнаружен файл прошивки Panel
    class AnswerUpgradePanel : public SimpleMessage
    {
    public:
        AnswerUpgradePanel(int size);
    };

    // Этим сообщением PLoader запрашивает порцию обновления размером SIZE_CHUNK
    class RequestPortionUpgradePanel : public SimpleMessage
    {
    public:
        RequestPortionUpgradePanel(uint16 num);
    };

    // Этим собщением DLoader отвечат на запрос порции обновления от PLoader
    class AnswerPortionUpgradePanel : public SimpleMessage
    {
    public:
        AnswerPortionUpgradePanel(int16 num, uint8 data[SIZE_CHUNK]);
    };
}
