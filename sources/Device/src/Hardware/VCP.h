#pragma once
#include "defines.h"
#include <usbd_def.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup VCP
 *  @brief Virtual Com Port
 *  @{
 */
 
#define VCP_FLUSH()                       VCP::Flush()
#define VCP_SEND_DATA_SYNCH(buffer, size) VCP::SendDataSynch(buffer, size)
#define CONNECTED_TO_USB                  VCP::connectedToUSB
#define CABLE_USB_IS_CONNECTED            VCP::cableUSBisConnected

namespace VCP
{
    const int DEVICE_FS = 0;

    /// Инициализация
    void Init();

    void SendDataAsynch(uint8 *data, uint size);

    void SendDataSynch(const void *data, uint size = 0);
    /// Передаётся строка без символа окончания строки
    void SendStringAsynch(char *data);
    /// Передаётся строка с символом окончания строки
    void SendStringAsynchEOF(char *data);
    /// Передаётся строка без завершающего нуля
    void SendStringSynch(char *data);
    /// Эта строка передаётся с завершающими символами \\r\\n
    void SendFormatStringAsynch(char *format, ...);
    /// Эта строка передаётся с завершающими символами \\r\\n
    void SendFormatStringSynch(char *format, ...);

    void SendByte(uint8 data);

    void Flush();

    extern USBD_HandleTypeDef handleUSBD;

    extern bool connectedToUSB;

    extern bool cableUSBisConnected;
};



/** @}
 */
