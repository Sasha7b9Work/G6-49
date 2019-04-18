#include "stdafx.h"
#include "usbd_desc.h"
#include "usbd_cdc_interface.h"
#ifndef WIN32
#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"
#include "usbd_desc.h"
#include "Utils/Math.h"
#include <stdarg.h>
#include <cstring>
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
USBD_HandleTypeDef VCP::handleUSBD;
bool               VCP::cableUSBisConnected = false;
bool               VCP::connectedToUSB = false;

namespace VCP
{
    bool PrevSendingComplete();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VCP::Init()
{
    CPU::PCD::Init();

    USBD_Init(&handleUSBD, &VCP_Desc, DEVICE_FS);
    USBD_RegisterClass(&handleUSBD, &USBD_CDC);
    USBD_CDC_RegisterInterface(&handleUSBD, &USBD_CDC_fops);
    USBD_Start(&handleUSBD);
} 

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool VCP::PrevSendingComplete()
{
    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
    return pCDC->TxState == 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
#define SIZE_BUFFER_VCP 256     /// \todo если поставить размер буфера 512, то на ТЕ207 глюки
static uint8 buffSend[SIZE_BUFFER_VCP];
static int sizeBuffer = 0;

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendData(const void *_buffer, uint size)
{
    volatile USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;

    while (pCDC->TxState == 1) {};
    USBD_CDC_SetTxBuffer(&handleUSBD, (uint8 *)_buffer, (uint16)size);
    USBD_CDC_TransmitPacket(&handleUSBD);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendString(char *data)
{
    SendData((uint8 *)data, std::strlen(data));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendByte(uint8 byte)
{
    SendData(&byte, 1);
}
