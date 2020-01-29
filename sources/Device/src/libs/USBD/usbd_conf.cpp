#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP_d.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>
#include <usbd_core.h>


/*******************************************************************************
LL Driver Callbacks (PCD -> USB Device Library)
*******************************************************************************/

void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_SetupStage(static_cast<USBD_HandleTypeDef *>(hpcd->pData), reinterpret_cast<uint8_t *>(hpcd->Setup));

    USBD_HandleTypeDef *pdev = static_cast<USBD_HandleTypeDef*>(hpcd->pData);  //
    USBD_SetupReqTypedef request = pdev->request;                   //
                                                                    //
    
    static uint16 prevLength = 7;                                   //
                                                                    //
    if (request.wLength == 0)                                       //
    {                                                               //
        if (prevLength != 0)                                        //
        {                                                           //
            DVCP::cableUSBisConnected = true;                       // Это потому, что при включении прибора с подключенным шнуром
            DVCP::connectedToUSB = true;                            // GOVNOCODE Таким вот замысловатым образом определяем, что к нам подконнектился хост (
        }                                                           //
        else                                                        //
        {                                                           //
            DVCP::connectedToUSB = false;                           //
        }                                                           /// \todo Возможно, это не нужно делать
    }                                                               //
    prevLength = request.wLength;                                   //
    
}

void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  USBD_LL_DataOutStage(static_cast<USBD_HandleTypeDef *>(hpcd->pData), epnum, hpcd->OUT_ep[epnum].xfer_buff);
}

void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    USBD_LL_DataInStage(static_cast<USBD_HandleTypeDef *>(hpcd->pData), epnum, hpcd->IN_ep[epnum].xfer_buff);
    
    /// \todo здесь оказываемя после каждой передачи пакета. Можно отловить момент, когда передача закончена.
}

void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_SOF(static_cast<USBD_HandleTypeDef *>(hpcd->pData));
}

void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
{ 
    USBD_SpeedTypeDef speed = USBD_SPEED_FULL;

    /* Set USB Current Speed */
    switch(hpcd->Init.speed)
    {
    case PCD_SPEED_HIGH:
        speed = USBD_SPEED_HIGH;
        break;
    
    case PCD_SPEED_FULL:
        break;
    }
    USBD_LL_SetSpeed(static_cast<USBD_HandleTypeDef *>(hpcd->pData), speed);
  
    /* Reset Device */
    USBD_LL_Reset(static_cast<USBD_HandleTypeDef *>(hpcd->pData));
}

void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_Suspend(static_cast<USBD_HandleTypeDef *>(hpcd->pData));
}

void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_Resume(static_cast<USBD_HandleTypeDef *>(hpcd->pData));
}

void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    USBD_LL_IsoOUTIncomplete(static_cast<USBD_HandleTypeDef *>(hpcd->pData), epnum);
}

void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    USBD_LL_IsoINIncomplete(static_cast<USBD_HandleTypeDef *>(hpcd->pData), epnum);
}

void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_DevConnected(static_cast<USBD_HandleTypeDef *>(hpcd->pData));
}

void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_DevDisconnected(static_cast<USBD_HandleTypeDef *>(hpcd->pData));
}

/*******************************************************************************
                       LL Driver Interface (USB Device Library --> PCD)
*******************************************************************************/

USBD_StatusTypeDef  USBD_LL_Init (USBD_HandleTypeDef *pdev)
{
    return static_cast<USBD_StatusTypeDef>(HAL_PCD::USBD_LL_Init(pdev));
}

USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef *pdev)
{
    HAL_PCD_DeInit(static_cast<PCD_HandleTypeDef *>(pdev->pData));
    return USBD_OK; 
}

USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev)
{
    HAL_PCD_Start(static_cast<PCD_HandleTypeDef *>(pdev->pData));
    return USBD_OK; 
}

USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev)
{
    HAL_PCD_Stop(static_cast<PCD_HandleTypeDef *>(pdev->pData));
    return USBD_OK; 
}

USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef *pdev, 
                                  uint8_t  ep_addr,                                      
                                  uint8_t  ep_type,
                                  uint16_t ep_mps)
{
    HAL_PCD_EP_Open(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr, ep_mps, ep_type);
    return USBD_OK; 
}

USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    HAL_PCD_EP_Close(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr);
    return USBD_OK; 
}

USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    HAL_PCD_EP_Flush(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr);
    return USBD_OK; 
}

USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    HAL_PCD_EP_SetStall(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr);
    return USBD_OK; 
}

USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    HAL_PCD_EP_ClrStall(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr);
    return USBD_OK; 
}

uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    PCD_HandleTypeDef *hpcd = static_cast<PCD_HandleTypeDef *>(pdev->pData);
  
    if((ep_addr & 0x80) == 0x80)
    {
        return hpcd->IN_ep[ep_addr & 0x7F].is_stall; 
    }
    else
    {
        return hpcd->OUT_ep[ep_addr & 0x7F].is_stall; 
    }
}

USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev, uint8_t dev_addr)   
{
    HAL_PCD_SetAddress(static_cast<PCD_HandleTypeDef *>(pdev->pData), dev_addr);
    return USBD_OK; 
}

USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev, 
                                    uint8_t  ep_addr,                                      
                                    uint8_t  *pbuf,
                                    uint16_t  size)
{
    HAL_PCD_EP_Transmit(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr, pbuf, size);
    return USBD_OK;   
}

USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev, 
                                          uint8_t  ep_addr,                                      
                                          uint8_t  *pbuf,
                                          uint16_t  size)
{
    HAL_PCD_EP_Receive(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr, pbuf, size);
    return USBD_OK;   
}

uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev, uint8_t  ep_addr)  
{
    return HAL_PCD_EP_GetRxCount(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr);
}

void  USBD_LL_Delay(uint32_t delay)
{
    HAL_TIM::Delay(delay);
}
