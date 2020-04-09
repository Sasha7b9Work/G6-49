#pragma once
#include "Display/Colors.h"


#define ERROR_HANDLER() HAL::ErrorHandler()


struct CalibrationSettings;


struct HAL
{
	static void Init();

    static void ErrorHandler();
};


struct HAL_EEPROM
{
    // Стирает count секторов, начиная со стартового адреса основной прошивки
    static void EraseSectors(int count);

    static void WriteBuffer(uint address, uint8 *data, int size);
};


struct HAL_LTDC
{
    static void Init();

    static void SetColors(uint *clut, uint8 numColors);

    static void ToggleBuffers();

    static void FillRegion(int x, int y, int width, int height, Color color);

    static uint8 *GetBuffer();
};


struct HAL_SPI4
{
    static void Init();
    /// Переслать массив данных
    static bool Transmit(const void *buffer, int size, uint timeout);

    static bool Transmit(uint value, uint timeout);
    static bool Transmit(int value, uint timeout);
    /// Принять массив данных
    static bool Receive(void *recv, int size, uint timeout);
    /// Функция принимает size байт и сравнивает их с compared. Возвращает количество дефектных байт
    static uint ReceiveAndCompare(const void *compared, int size);
    /// Возвращает true, если прибор готов к обмену
    static bool IsReady();
    /// Ждать пока освободится устройство
    static void WaitFreedom();
    /// Ожидание перехода сигнала готовности из состояния "занят" в состояние "свободен"
    static void WaitFalling();
};


struct HAL_TIM
{
    static uint TimeMS();

    static void Delay(uint timeMS);
};


// Используетя для тиков
struct HAL_TIM2
{
    static void Init();

    static void DeInit();

    static uint TimeUS();

    static uint GetTicks();
    /// Запускает счётчик для измерения малых отрезков времени
    static void StartMultiMeasurement();
};


// Для таймеров
struct HAL_TIM3
{
    static void Init();

    static void StartIT(uint period);

    static void StopIT();

    static void DeInit();
};


struct HAL_TIM4
{
    static void Init(void (*func)());
    /// Завести таймер, который остановится в timeStop мс
    static void Start(uint timeStopMS);

    static void Stop();

    static void ElapsedCallback();
};
