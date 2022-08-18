// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Generator/Generator_d.h"


struct CalibrationSettings;


#define ERROR_HANDLER() HAL::ErrorHandler()


namespace HAL
{
    void Init();

    void ErrorHandler();
};


namespace HAL_CRC32
{
    uint Calculate(const uint8 *address, uint size);

    void Init();
};


namespace HAL_EEPROM
{
    void Init();

    namespace Signal
    {
        void Save(const Chan &, uint16 data[DGenerator::DDS_NUM_POINTS]);

        uint16 *Get(const Chan &);
    };
};


namespace HAL_HCD
{
    void Init();

    void IRQHandler();

    int USBH_LL_Init(void *usbh_HandleTypeDef);

    int USBH_LL_SetToggle(uint8 pipe, uint8 toggle);

    uint8 USBH_LL_GetToggle(uint8 pipe);
};


namespace HAL_I2C1
{
    void Init();

    void Transmit(uint8 address, uint8 data[3]);
};


namespace HAL_PCD
{
    void Init();

    void IRQHandler();

    int USBD_LL_Init(void *usbd_HandleTypeDef);
};


namespace HAL_SPI1
{
    void Init();

    bool Receive(void *buffer, int size, int timeout);

    bool Transmit(void *buffer, int size, int timeout);

    bool Transmit(int value, int timeout);
};


namespace HAL_SPI3
{
    void Init();
    // Запускает счётчик для измерения малых отрезков времени
    void Transmit(uint8 *buffer, uint16 num);
};


namespace HAL_TIM
{
    uint TimeMS();

    void Delay(uint timeMS);
};


namespace HAL_TIM2
{
    void Init();

    uint GetTicks();

    void StartMultiMeasurement();

    uint TimeUS();
};


namespace HAL_TIM3
{
    void Init();

    void StartIT(uint period);

    void StopIT();
};
