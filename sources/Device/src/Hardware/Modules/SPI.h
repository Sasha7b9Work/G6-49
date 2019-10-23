#pragma once



namespace SPI1_
{
    void Init();

    bool Receive(void *buffer, uint size, uint timeout);

    bool Transmit(void *buffer, uint size, uint timeout);

    bool Transmit(uint value, uint timeout);
};
