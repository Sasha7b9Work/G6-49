// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


#define ERROR_HANDLER() HAL::ErrorHandler()


struct CalibrationSettings;


namespace HAL
{
    void Init();

    void ErrorHandler();
};


namespace HAL_CRC32
{
    void Init();

    uint CalculateALIGN32(void *address, int size);
};


namespace HAL_EEPROM
{
    // Стирает count секторов, начиная со стартового адреса основной прошивки
    void EraseSectors(int count);

    void WriteBuffer(int address, const uint8 *data, int size);
};


namespace HAL_LTDC
{
    void Init();

    void SetColors(uint *clut, uint8 numColors);

    void ToggleBuffers();

    void FillRegion(int x, int y, int width, int height, const Color &);

    uint8 *GetBuffer();
};


namespace HAL_SPI4
{
    void Init();

    // Переслать массив данных
    bool Transmit(const void *buffer, int size, int timeout);

    bool Transmit(uint value, int timeout);

    bool Transmit(int value, int timeout);

    // Принять массив данных
    bool Receive(void *recv, int size, int timeout);

    // Функция принимает size байт и сравнивает их с compared. Возвращает количество дефектных байт
    uint ReceiveAndCompare(const void *compared, int size);

    // Возвращает true, если прибор готов к обмену
    bool IsReady();

    // Ожидание перехода сигнала готовности из состояния "занят" в состояние "свободен"
    void WaitRelease();
};


namespace HAL_TIM
{
    uint TimeMS();

    void Delay(uint timeMS);
};


// Используетя для тиков
namespace HAL_TIM2
{
    void Init();

    void DeInit();

    uint TimeUS();

    uint GetTicks();

    // Запускает счётчик для измерения малых отрезков времени
    void StartMultiMeasurement();
};


// Для таймеров
namespace HAL_TIM3
{
    void Init();

    void StartIT(uint period);

    void StopIT();

    void DeInit();
};


namespace HAL_TIM4
{
    void Init(void (*func)());
    // Завести таймер, который остановится в timeStop мс
    void Start(uint timeStopMS);

    void Stop();

    void ElapsedCallback();
};
