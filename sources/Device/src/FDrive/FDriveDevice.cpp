#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Log.h"
#include "FDriveDevice.h"
#include "Hardware/CPU/CPU.h"
#include "Utils/Console.h"
#include "usbh_diskio.h"
#include "usbh_conf.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
USBH_HandleTypeDef FDrive::hUSB_Host;

static char USBDISKPath[4];
/// true, если флешка подключена
volatile static bool isConnected = false;


struct State
{
    enum E
    {
        Disconnected,   ///< Начальное значение после старта
        NeedMount       ///< Обнаружена подключенная флешка, требуется монтирование
    } value;
};

static State::E state = State::Disconnected;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// В эту функцию попадаем при каждом событии на OTG FS
static void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    switch(id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            Console::AddString("HOST_USER_SELECT_CONFIGURATION");
            break;

        case HOST_USER_CLASS_ACTIVE:
             Console::AddString("HOST_USER_CLASS_ACTIVE");
            break;

        case HOST_USER_CLASS_SELECTED:
            Console::AddString("HOST_USER_CLASS_SELECTED");
            break;

        case HOST_USER_CONNECTION:
            Console::AddString("HOST_USER_CONNECTION");
            break;

        case HOST_USER_DISCONNECTION:
            Console::AddString("HOST_USER_DISCONNECTION");
            isConnected = false;
            break;

        default:
            Console::AddString("default");
            break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Init()
{
    CPU::HCD::Init();

    if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == FR_OK)
    {
        USBH_StatusTypeDef res = USBH_Init(&hUSB_Host, USBH_UserProcess, 0);
        res = USBH_RegisterClass(&hUSB_Host, USBH_MSC_CLASS);
        res = USBH_Start(&hUSB_Host);
    }
    else
    {
        LOG_ERROR("Can not %s", __FUNCTION__);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Update()
{
    USBH_Process(&hUSB_Host);
}
