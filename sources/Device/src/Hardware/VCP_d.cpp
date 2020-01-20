#include "usbd_desc.h"
#include "usbd_cdc_interface.h"
#include "defines.h"
#include "common/Message.h"
#include "Hardware/VCP_d.h"
#include "Hardware/HAL/HAL.h"
#include "usbd_desc.h"
#include "Utils/Math.h"
#include <stdarg.h>
#include <cstring>



USBD_HandleTypeDef DVCP::handleUSBD;
bool               DVCP::cableUSBisConnected = false;
bool               DVCP::connectedToUSB = false;


void DVCP::Init()
{
    HAL_PCD::Init();

    USBD_Init(&handleUSBD, &VCP_Desc, DEVICE_FS);
    USBD_RegisterClass(&handleUSBD, &USBD_CDC);
    USBD_CDC_RegisterInterface(&handleUSBD, &USBD_CDC_fops);
    USBD_Start(&handleUSBD);
} 


bool DVCP::PrevSendingComplete()
{
    USBD_CDC_HandleTypeDef *pCDC = static_cast<USBD_CDC_HandleTypeDef *>(handleUSBD.pClassData);
    return pCDC->TxState == 0;
}


#define SIZE_BUFFER_VCP 256     /// \todo если поставить размер буфера 512, то на ТЕ207 глюки
//static uint8 buffSend[SIZE_BUFFER_VCP];
//static int sizeBuffer = 0;


void DVCP::SendData(const void *_buffer, uint size)
{
    volatile USBD_CDC_HandleTypeDef *pCDC = static_cast<USBD_CDC_HandleTypeDef *>(handleUSBD.pClassData);

    while (pCDC->TxState == 1) {};
    USBD_CDC_SetTxBuffer(&handleUSBD, static_cast<uint8 *>(const_cast<void *>(_buffer)), static_cast<uint16>(size));
    USBD_CDC_TransmitPacket(&handleUSBD);
}


void DVCP::SendString(char *data)
{
    SendData(reinterpret_cast<uint8 *>(data), std::strlen(data));
}


void DVCP::SendStringEOF(char *data)
{
    SendString(data);
    //SendByte(0x0d);
}


void DVCP::SendByte(uint8 byte)
{
    SendData(&byte, 1);
}


void DVCP::Handler::Processing(SimpleMessage *msg)
{
    DVCP::SendData(msg->Data(5), msg->TakeWord());
}
