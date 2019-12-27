#pragma once
#include "Generator/Generator_d.h"
#include <stm32f4xx_hal.h>


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
    static void Init();

    static void SaveSettings(CalibrationSettings *settings);

    static void LoadSettings(CalibrationSettings *settings);

    struct Signal
    {
        static void Save(Chan::E ch, uint16 data[DGenerator::DDS_NUM_POINTS]);

        static uint16 *Get(Chan::E ch);
    };
};


struct HAL_HCD
{
    static void Init();

    static void IRQHandler();

    static int USBH_LL_Init(void *usbh_HandleTypeDef);

    static int USBH_LL_SetToggle(uint8 pipe, uint8 toggle);

    static uint8 USBH_LL_GetToggle(uint8 pipe);
};


struct HAL_PCD
{
    static void Init();

    static PCD_HandleTypeDef handle;
};


struct HAL_SPI1
{
    static void Init();

    static bool Receive(void *buffer, uint size, uint timeout);

    static bool Transmit(void *buffer, uint size, uint timeout);

    static bool Transmit(uint value, uint timeout);
};


struct HAL_TIM2
{
    static void Init();

};


struct HAL_TIM3
{
    static void Init();

    static void StartIT(uint period);

    static void StopIT();
};
