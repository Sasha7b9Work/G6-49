#pragma once



struct HAL_SPI1
{
    static void Init();

    static bool Receive(void *buffer, uint size, uint timeout);

    static bool Transmit(void *buffer, uint size, uint timeout);

    static bool Transmit(uint value, uint timeout);
};
