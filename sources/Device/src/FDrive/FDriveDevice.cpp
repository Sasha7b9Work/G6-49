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

static FATFS FatFS;

static char USBDISKPath[4];
/// true, если флешка подключена
volatile static bool isConnected = false;
/// Количество байт для передачи в Interface
static uint16 numBytesForSend = 0;
/// Начало буфера данных для передачи в Interface
static uint8 *bufferForSend = 0;
/// Если true, то устройство занято и обмен с интерфейсом запрещён
static bool isBusy = false;


struct State
{
    enum E
    {
        Disconnected,   ///< Начальное значение после старта
        NeedMount,      ///< Обнаружена подключенная флешка, требуется монтирование
        Connected,      ///< Флешка подсоединена и примонтирована
        NeedUnmount     ///< Требуется отмонтировать
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
            break;

        case HOST_USER_CLASS_ACTIVE:
            state = State::NeedMount;
            break;

        case HOST_USER_CLASS_SELECTED:
            break;

        case HOST_USER_CONNECTION:
            break;

        case HOST_USER_DISCONNECTION:
            state = State::NeedUnmount;
            break;

        default:
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

    if(state == State::NeedMount)
    {
        isBusy = true;

        if(f_mount(&FatFS, USBDISKPath, 0) == FR_OK)
        {
            numBytesForSend = 1 + 1;
            bufferForSend = (uint8 *)malloc(numBytesForSend);
            bufferForSend[0] = Command::FDrive_Mount;
            bufferForSend[1] = 1;
        }

        state = State::Connected;

        isBusy = false;
    }
    else if(state == State::NeedUnmount)
    {
        isBusy = true;

        f_mount(0, "", 0);

        numBytesForSend = 1 + 1;
        bufferForSend = (uint8 *)malloc(numBytesForSend);
        bufferForSend[0] = Command::FDrive_Mount;
        bufferForSend[1] = 0;

        state = State::Disconnected;

        isBusy = false;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint16 FDrive::NumBytesForSend()
{
    if(isBusy)
    {
        return 0;
    }
    return numBytesForSend;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *FDrive::GetDataForSend(uint8 *buffer)
{
    memcpy(buffer, bufferForSend, numBytesForSend);
    free(bufferForSend);
    numBytesForSend = 0;
    return buffer;
}
