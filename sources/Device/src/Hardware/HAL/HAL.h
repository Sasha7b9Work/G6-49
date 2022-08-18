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

    struct Signal
    {
        static void Save(const Chan &, uint16 data[DGenerator::DDS_NUM_POINTS]);

        static uint16 *Get(const Chan &);

    private:
        // Возвращает адрес EEPROM, куда надо сохранять данные этого канала
        static uint AddressForData(const Chan &);
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


struct HAL_I2C1
{
    static void Init();

    static void Transmit(uint8 address, uint8 data[3]);
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
