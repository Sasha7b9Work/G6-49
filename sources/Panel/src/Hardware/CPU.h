#pragma once
#include "common/stm32/429/stm429.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
class _LTDC_
{
public:
    static void SetColors(uint clut[], uint numColors);
};
*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CPU : public STM429
{
public:

    static void Config();

    /// Установка каллбэка для клавиатуры
    static void SetCallbackKeyboard(void (*func)());

    static void InitKeyboardInputs(uint16 sl[], char portSL[], int numSL, uint16 rl[], char portRL[], int numRL);

    static void SetFrontBuffer(uint frontBuffer);

    class _LTDC_
    {
    public:
        static void SetColors(uint clut[], uint numColors);
    };

    class _SPI4_
    {
    public:
        static void Transmit(uint8 *buffer, uint16 size, uint timeOut);

        static void TransmitReceive(uint8 *trans, uint8 *receiv, uint16 size, uint timeOut);
    };

private:
    /// Инициализация SPI для связи с основным процессором
    static void InitSPI4();

    static void InitTIM2();

    static void InitTIM5();

    static void InitHardware();
    /// Инициализация дисплея
    static void InitLTDC();
    /// Инициализация шины для связи с ПЛИС
    static void InitFSMC();

    static void EnablePeriphery();
};


#ifdef __cplusplus
extern "C" {
#endif

void LTDC_IRQHandler();
void TIM3_IRQHandler();

#ifdef __cplusplus
}
#endif
