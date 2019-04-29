#pragma once
#include "defines.h"
#include <usbd_def.h>


class SimpleMessage;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace VCP
{
    const int DEVICE_FS = 0;

    /// Инициализация
    void Init();

    void SendData(const void *data, uint size = 0);
    /// Передаётся строка без завершающего символа
    void SendString(char *data);
    /// Передача строки с символом конца строки
    void SendStringEOF(char *data);

    void SendByte(uint8 data);

    extern USBD_HandleTypeDef handleUSBD;

    extern bool connectedToUSB;

    extern bool cableUSBisConnected;

    namespace Handler
    {
        void Processing(SimpleMessage *msg);
    }
};
