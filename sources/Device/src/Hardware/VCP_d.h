// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class SimpleMessage;


struct DVCP
{
    static const int DEVICE_FS = 0;

    // �������������
    static void Init();

    static void SendData(const void *data, uint size = 0);
    // ��������� ������ ��� ������������ �������
    static void SendString(char *data);
    // �������� ������ � �������� ����� ������
    static void SendStringEOF(char *data);

    static void SendByte(uint8 data);

    static void *handle;

    struct Handler
    {
        static void Processing(SimpleMessage *msg);
    };

    static void SetConnectedToUSB(bool connected);

    static void SetCableUSBisConnected(bool connected);

private:
    bool PrevSendingComplete();

    static bool connectedToUSB;

    static bool cableUSBisConnected;
};
