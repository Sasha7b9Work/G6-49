#include "defines.h"
#include "common/Messages.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP_d.h"
#include <usbd_cdc.h>



static USBD_CDC_LineCodingTypeDef LineCoding =
{
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
};

#define APP_RX_DATA_SIZE  64
static uint8_t UserRxBuffer[APP_RX_DATA_SIZE];/* Received Data over USB are stored in this buffer */


static int8_t CDC_Itf_Init     ();
static int8_t CDC_Itf_DeInit   ();
static int8_t CDC_Itf_Control  (uint8 cmd, uint8* pbuf, uint16 length);
static int8_t CDC_Itf_Receive  (uint8* pbuf, uint *Len);


USBD_CDC_ItfTypeDef USBD_CDC_fops = 
{
    CDC_Itf_Init,
    CDC_Itf_DeInit,
    CDC_Itf_Control,
    CDC_Itf_Receive
};



static void SetAttributeConnected()
{
    DVCP::SetCableUSBisConnected(true);
    DVCP::SetConnectedToUSB(false);
}


static int8_t CDC_Itf_Init()
{
    USBD_CDC_SetRxBuffer(reinterpret_cast<USBD_HandleTypeDef *>(DVCP::handle), UserRxBuffer); //-V2571
    Timer::SetAndStartOnce(Timer::Type::USB, SetAttributeConnected, 100);   /** \todo Задержка введена для того, чтобы не было ложных срабатываний в 
                                                                 usbd_conf.c:HAL_PCD_SetupStageCallback при определении подключения хоста */
    return (USBD_OK);
}


static int8_t CDC_Itf_DeInit()
{
    DVCP::SetCableUSBisConnected(false);
    DVCP::SetConnectedToUSB(false);
    return (USBD_OK);
}


static int8_t CDC_Itf_Control (uint8 cmd, uint8* pbuf, uint16)
{ 
    switch (cmd)
    {
    case CDC_SEND_ENCAPSULATED_COMMAND:
        /* Add your code here */
        break;
        
    case CDC_GET_ENCAPSULATED_RESPONSE:
        /* Add your code here */
        break;

    case CDC_SET_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_GET_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_CLEAR_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_SET_LINE_CODING:
        LineCoding.bitrate    = static_cast<uint>(pbuf[0] | (pbuf[1] << 8) | (pbuf[2] << 16) | (pbuf[3] << 24)); //-V2563
        LineCoding.format     = pbuf[4]; //-V2563
        LineCoding.paritytype = pbuf[5]; //-V2563
        LineCoding.datatype   = pbuf[6]; //-V2563
        break;

    case CDC_GET_LINE_CODING:
        pbuf[0] = static_cast<uint8_t>(LineCoding.bitrate); //-V2563
        pbuf[1] = static_cast<uint8_t>(LineCoding.bitrate >> 8); //-V2563
        pbuf[2] = static_cast<uint8_t>(LineCoding.bitrate >> 16); //-V2563
        pbuf[3] = static_cast<uint8_t>(LineCoding.bitrate >> 24); //-V2563
        pbuf[4] = LineCoding.format; //-V2563
        pbuf[5] = LineCoding.paritytype; //-V2563
        pbuf[6] = LineCoding.datatype; //-V2563
        /* Add your code here */
        break;

    case CDC_SET_CONTROL_LINE_STATE:
        /* Add your code here */
        break;

    case CDC_SEND_BREAK:
        /* Add your code here */
        break;    
    }
  
    return (USBD_OK);
}


static int8_t CDC_Itf_Receive(uint8 *buffer, uint *length) //-V2009 //-V2558
{
    Message::SCPI::Data(buffer, static_cast<int>(*length)).Transmit();

    USBD_CDC_ReceivePacket(reinterpret_cast<USBD_HandleTypeDef *>(DVCP::handle)); //-V2571

    return USBD_OK;
}
