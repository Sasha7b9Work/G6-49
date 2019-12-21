#pragma once
#include "defines.h"
#include <usbd_def.h>


class SimpleMessage;


struct DVCP
{
    static const int DEVICE_FS = 0;

    /// Инициализация
    static void Init();

    static void SendData(const void *data, uint size = 0);
    /// Передаётся строка без завершающего символа
    static void SendString(char *data);
    /// Передача строки с символом конца строки
    static void SendStringEOF(char *data);

    static void SendByte(uint8 data);

    static USBD_HandleTypeDef handleUSBD;

    static bool connectedToUSB;

    static bool cableUSBisConnected;

    struct Handler
    {
        static void Processing(SimpleMessage *msg);
    };

private:
    bool PrevSendingComplete();
};
