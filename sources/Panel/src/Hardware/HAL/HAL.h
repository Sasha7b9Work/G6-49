// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


#define ERROR_HANDLER() HAL::ErrorHandler()


struct CalibrationSettings;


struct HAL
{
	static void Init();

    static void ErrorHandler();
};


struct HAL_CRC32
{
    static uint Calculate(const void *address, uint size);

    static void Init();
};


struct HAL_DAC2
{
	static void Init();

    static void Start();

    static void Stop();

    static bool IsRun();
};


struct HAL_EEPROM
{
    static void SaveSettings(CalibrationSettings *settings);

    static void LoadSettings(CalibrationSettings *settings);
};


// Шина для связи с ПЛИС
struct HAL_FSMC
{
    static void Init();
};


struct HAL_LTDC
{
    static void Init(uint frontBuffer, uint backBuffer);

    static void SetColors(uint *clut, uint8 numColors);

    static void ToggleBuffers();

    static void FillRegion(int x, int y, int width, int height, Color color);
};


struct HAL_SPI4
{
    static void Init();
    
    // Переслать массив данных
    static bool Transmit(const void *buffer, int size, int timeout);

    static bool Transmit(int value, int timeout);
    
    // Принять массив данных
    static bool Receive(void *recv, int size, int timeout);
    
    // Функция принимает size байт и сравнивает их с compared. Возвращает количество дефектных байт
    static uint ReceiveAndCompare(const void *compared, int size);
    
    // Возвращает true, если прибор готов к обмену
    static bool IsReady();
  
    // Ожидание перехода сигнала готовности из состояния "занят" в состояние "свободен"
    static void WaitFalling();
};


struct HAL_TIM
{
    static uint TimeMS();

    static void Delay(uint timeMS);
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


struct HAL_TIM4
{
    static void Init(void (*func)());
    
    // Завести таймер, который остановится в timeStop мс
    static void Start(uint timeStopMS);

    static void Stop();

    static void ElapsedCallback();
};
