#pragma once


class SPI4_
{
friend class CPU;
private:
    static void Init();
public:
    /// Переслать массив данных
    static bool Transmit(const void *buffer, uint size);
    /// Принять массив данных
    static bool Receive(void *recv, uint size);
    /// Возвращает true, если прибор готов к обмену
    static bool IsReady();
    /// Ждать пока освободится устройство
    static void WaitFreedom();
    /// Ожидание перехода сигнала готовности из состояния "занят" в состояние "свободен"
    static void WaitFalling();
};
