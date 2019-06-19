#pragma once


namespace SPI4_
{
    void Init();
    /// Переслать массив данных
    bool Transmit(const void *buffer, uint size, uint timeout);

    bool Transmit(uint value, uint timeout);
    /// Принять массив данных
    bool Receive(void *recv, uint size, uint timeout);
    /// Функция принимает size байт и сравнивает их с compared. Возвращает количество дефектных байт
    uint ReceiveAndCompare(const void *compared, uint size);
    /// Возвращает true, если прибор готов к обмену
    bool IsReady();
    /// Ждать пока освободится устройство
    void WaitFreedom();
    /// Ожидание перехода сигнала готовности из состояния "занят" в состояние "свободен"
    void WaitFalling();
};
