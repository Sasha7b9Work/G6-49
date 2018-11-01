#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Log.h"
#include "FDrive.h"
#include "usbh_diskio.h"
#include "usbh_conf.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
USBH_HandleTypeDef FDrive::hUSB_Host;

static char USBDISKPath[4];
/// Если true - флешка обнаружена, её нужно монтировать.
volatile static bool needMount = false;
/// true, если флешка подключена
volatile static bool isConnected = false;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// В эту функцию попадаем при каждом событии на OTG FS
static void USBH_Process(USBH_HandleTypeDef *, uint8 id);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void USBH_Process(USBH_HandleTypeDef *, uint8 id)
{
    switch(id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;

        case HOST_USER_CLASS_ACTIVE:
             needMount = true;

            /*
            if (f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 1) != FR_OK)
            {
                display.ShowWarning(WrongFileSystem);
            }
            else
            {
                gFlashDriveIsConnected = true;
                FM_Init();
                Menu::ChangeStateFlashDrive();
            }
            */
            break;

        case HOST_USER_CLASS_SELECTED:
            break;

        case HOST_USER_CONNECTION:
            f_mount(NULL, (TCHAR const*)"", 0);
            break;

        case HOST_USER_DISCONNECTION:
            isConnected = false;
            break;

        default:
            break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Init()
{
    if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == FR_OK)
    {
        USBH_StatusTypeDef res = USBH_Init(&hUSB_Host, USBH_Process, 0);
        res = USBH_RegisterClass(&hUSB_Host, USBH_MSC_CLASS);
        res = USBH_Start(&hUSB_Host);
    }
    else
    {
        LOG_ERROR("Can not %s", __FUNCTION__);
    }
}
