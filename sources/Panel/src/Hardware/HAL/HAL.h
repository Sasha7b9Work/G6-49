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
	static void StartDMA(uint prescaler);
	static void StopDMA();
private:
	static void ConfigTIM7(uint prescaler);
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


struct HAL_GPIO
{
    static void WritePin(char port, uint16 maskPin, bool state);

    static bool ReadPin(char port, uint16 maskPin);
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
    /// Переслать массив данных
    static bool Transmit(const void *buffer, uint size, uint timeout);

    static bool Transmit(uint value, uint timeout);
    /// Принять массив данных
    static bool Receive(void *recv, uint size, uint timeout);
    /// Функция принимает size байт и сравнивает их с compared. Возвращает количество дефектных байт
    static uint ReceiveAndCompare(const void *compared, uint size);
    /// Возвращает true, если прибор готов к обмену
    static bool IsReady();
    /// Ждать пока освободится устройство
    static void WaitFreedom();
    /// Ожидание перехода сигнала готовности из состояния "занят" в состояние "свободен"
    static void WaitFalling();
};


// Используетя для тиков
struct HAL_TIM2
{
    static void Init();

    static void DeInit();
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
