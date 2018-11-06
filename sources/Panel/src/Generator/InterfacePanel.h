#pragma once
#include "Utils/Buffer.h"


class Interface
{
public:
    /// Для приёма сообщений от устройства
    static void Update();
    /// Принять и обработать данные от ПЛИС
    static void ProcessDataFPGA();
    /// Заслать в генератор данные
    static void Send(const uint8 *buffer, uint size);

    static void Send(const Buffer &buffer);
    /// Принять numBytes байт от ПЛИС и выполнить их
    static void ReceiveAndRun(uint16 numBytes);
};
