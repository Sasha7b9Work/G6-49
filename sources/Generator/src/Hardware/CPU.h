#pragma once
#include "Hardware/stm407.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum GeneratorPin
{
    AD9952_SPI3_CSA,
    AD9952_SPI3_CSB,
    AD9952_IO_UPD,
    AD9952_IOSYNA,
    AD9952_IOSYNB,
    AD9952_RES_DDS,
    Pin_P1_AmplifierA,
    Pin_P2_AmplifierB,
    Pin_P3_OutA,
    Pin_P4_OutB,
    AD5697_Offset,      ///< Управление смещением выхода AD9952
    AD5697_Freq,        ///< Управление параметрами частотомера
    AD5697_D_RSA,
    AD5697_D_RSB,
    FPGA_WR_RG,
    FPGA_CLK_RG,
    FPGA_DT_RG,
    FPGA_A0_RG,
    FPGA_A1_RG,
    FPGA_A2_RG,
    FPGA_A3_RG,
    FREQ_METER_RESIST,
    FREQ_METER_COUPLE,
    FREQ_METER_FILTR,
    NumPins
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CPU : public STM407
{
public:
    static void Init();
    /// Установить на PA2 признак того, что процессор занят
    static void SetBusy();
    /// Установить на PA2 признак того, что процессор свободен
    static void SetReady();
    
    static  void WritePin(GeneratorPin pin, bool set);

private:

    static void EnablePeriphery();

    static void InitGPIOS();
    /// Начальная инициализация управляющих выводов
    static void InitPins();

};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif

    /// Прерывание на таймер
    void TIM3_IRQHandler();

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
