// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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
    static void Init();

    static uint CalculateALIGN32(void *address, int size);
};


struct HAL_EEPROM
{
    // ������� count ��������, ������� �� ���������� ������ �������� ��������
    static void EraseSectors(int count);

    static void WriteBuffer(int address, const uint8 *data, int size);
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

    static bool Receive(void *buffer, int size, int timeout);

    static bool Transmit(void *buffer, int size, int timeout);

    static bool Transmit(int value, int timeout);
};


struct HAL_SPI3
{
    static void Init();
    // ��������� ������� ��� ��������� ����� �������� �������
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
