#pragma once
#include "Generator/Generator_d.h"


struct CalibrationSettings;


#define ERROR_HANDLER() HAL::ErrorHandler()


struct HAL
{
    static void Init();

    static void ErrorHandler();

private:

    static void SystemClockConfig();

    static void EnablePeriphery();
};



struct HAL_CRC32
{
    static uint Calculate(const uint8 *address, uint size);

    static void Init();
};


struct HAL_EEPROM
{
    // јдрес временного сектора, которым мы воспользуемс€, чтобы записать прошивку Panel
    static const uint ADDRESS_SECTOR_TEMP = 0x080c0000;

    // —тирает count секторов, начина€ со стартового адреса основной прошивки
    static void EraseSectors(int count);

    // —тирает временный сектор
    static void EraseSectorTemp();

    static void WriteBuffer(uint address, uint8 *data, int size);
};


struct HAL_HCD
{
    static void Init();

    static void IRQHandler();

    static int USBH_LL_Init(void *usbh_HandleTypeDef);

    static int USBH_LL_SetToggle(uint8 pipe, uint8 toggle);

    static uint8 USBH_LL_GetToggle(uint8 pipe);
};


struct HAL_SPI1
{
    static void Init();

    static bool Receive(void *buffer, int size, uint timeout);

    static bool Transmit(void *buffer, int size, uint timeout);

    static bool Transmit(uint value, uint timeout);
};


struct HAL_SPI3
{
    static void Init();
    /// «апускает счЄтчик дл€ измерени€ малых отрезков времени
    static void Transmit(uint8 *buffer, uint16 num);
};


struct HAL_TIM
{
    static uint TimeMS();

    static void Delay(uint timeMS);
};


struct HAL_TIM2
{
    static void Init();

    static uint GetTicks();

    static void StartMultiMeasurement();

    static uint TimeUS();
};


struct HAL_TIM3
{
    static void Init();

    static void StartIT(uint period);

    static void StopIT();
};
