#pragma once


class SPI4_
{
friend class CPU;
private:
    static void Init();
public:
    /// Переслать массив данных
    static bool Transmit(const void *buffer, uint size, uint timeout = 200);
    /// Принять массив данных
    static bool Receive(void *recv, uint size, uint timeout = 200);
    /// Функция принимает size байт и сравнивает их с compared. Возвращает true, если байты идентичны
    static bool ReceiveAndCompare(const void *compared, uint size);
    /// Возвращает true, если прибор готов к обмену
    static bool IsReady();
    /// Ждать пока освободится устройство
    static void WaitFreedom();
    /// Ожидание перехода сигнала готовности из состояния "занят" в состояние "свободен"
    static void WaitFalling();
};
