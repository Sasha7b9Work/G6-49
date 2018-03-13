#pragma once
#include "Hardware/Controls.h"
#include "Hardware/stm32/4XX/stm429.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RTC_GET_PACKED_TIME() {}
#define RTC_SET_TIME_AND_DATA(d, m, y, h, min, s)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CPU : public STM429
{
public:

    static void Init();

    //----------------------------------------------------------------------------------------------------------------------------------- Keyboard ---
    class Keyboard
    {
    friend class CPU;

    private:

        static void Init();

    public:
        /// Установка каллбэка для клавиатуры
        static void SetCallback(void(*func)());

        static void InitInputs(uint16 sl[], char portSL[], int numSL, uint16 rl[], char portRL[], int numRL);
        /// Возвращает true, если буфер пуст
        static bool BufferIsEmpty();
        /// Возвращает следующий орган управления, если таковой имеется
        static StructControl GetNextControl();

        //----------------------------------------------------------------------------------------------------------------------------------- TIM4 ---
        class TIM4_
        {
        public:
            /// Завести таймер, который остановится в timeStop мс
            static void Start(uint timeStopMS);

            static void Stop();

            static void ElapsedCallback(void *htim);
        };

    private:

        static void Update();
    };

    //--------------------------------------------------------------------------------------------------------------------------------------- SPI4 ---
    // SPI для связи с основным процессором
    class SPI4_
    {
    friend class CPU;
    private:
        static void Init();
    public:
        /// Переслать массив данных
        static void Transmit(uint8 *buffer, uint16 size, uint timeOut);
        /// Переслать массив данных с одновременным приёмом второго массива
        static void TransmitReceive(uint8 *trans, uint8 *receiv, uint16 size, uint timeOut);
        /// Возвращает true, если интерфейс занят - процессор не может приниммать команды
        static bool IsBusy();
    };

    //-------------------------------------------------------------------------------------------------------------------------------------- GPIO ----
    class GPIO_
    {
    public:
        static void WritePin(char port, uint16 maskPin, bool state);

        static bool ReadPin(char port, uint16 maskPin);
    };

    //-------------------------------------------------------------------------------------------------------------------------------------- FLASH ---
    class FLASH_
    {
    public:
        static void SaveSettings();
        static void LoadSettings();
    };

    //--------------------------------------------------------------------------------------------------------------------------------------- FSMC ---
    // Шина для связи с ПЛИС
    class FSMC
    {
    friend class CPU;

    private:
        static void Init();
    };

private:

        static void SystemClockConfig();
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif

    /// Прерывание на таймер. Timer.cpp
    void TIM3_IRQHandler();
    /// Прерывание на опрос клавиатуры. CPU_Keyboard.cpp
    void TIM4_IRQHandler();

    void NMI_Handler();
    void HardFault_Handler();
    void MemManage_Handler();
    void BusFault_Handler();
    void UsageFault_Handler();
    void SVC_Handler();
    void DebugMon_Handler();
    void PendSV_Handler();
    void SysTick_Handler();
    
#ifdef __cplusplus
}
#endif
