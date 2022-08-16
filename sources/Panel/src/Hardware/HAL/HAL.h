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
    uint Calculate(const void *address, uint size);

    void Init();
};


namespace HAL_DAC2
{
    void Init();

    void Start();

    void Stop();

    bool IsRun();
};


namespace HAL_EEPROM
{
    void SaveSettings(CalibrationSettings *settings);

    void LoadSettings(CalibrationSettings *settings);
};


// Шина для связи с ПЛИС
namespace HAL_FSMC
{
    void Init();
};


namespace HAL_LTDC
{
    void Init(uint frontBuffer, uint backBuffer);

    void SetColors(uint *clut, uint8 numColors);

    void ToggleBuffers();

    void FillRegion(int x, int y, int width, int height, const Color &);
};


namespace HAL_SPI4
{
    void Init();

    // Переслать массив данных
    bool Transmit(const void *buffer, int size, int timeout);

    bool Transmit(int value, int timeout);

    // Принять массив данных
    bool Receive(void *recv, int size, int timeout);

    // Функция принимает size байт и сравнивает их с compared. Возвращает количество дефектных байт
    uint ReceiveAndCompare(const void *compared, int size);

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
