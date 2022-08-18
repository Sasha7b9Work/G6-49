// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace ComPort
{
    bool Open();
    void Close();
    bool IsOpened();
    void Send(pchar);
    int Receive(char *buffer, int size);
};
