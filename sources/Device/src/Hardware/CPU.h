#pragma once
#include "defines.h"


/// Порты записи
struct GeneratorWritePin
{
    enum E
    {
        AD9952_SPI3_CSA,
        AD9952_SPI3_CSB,
        AD9952_IO_UPD,
        AD9952_IOSYNA,
        AD9952_IOSYNB,
        AD9952_RES_DDS,
        Pin_P1_AmplifierA,
        Pin_P2_AmplifierB,
        Pin_OutA,
        Pin_OutB,
        AD5697_Offset,      
        AD5697_Freq,        
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
        FREQ_METER_CLK,     
        D0,
        D1,
        D2,
        D3,
        D4,
        D5,
        D6,
        D7,
        FPGA_WR_DATA,
        Count
    } value;
    GeneratorWritePin(E v) : value(v) {};
    operator uint8() const { return static_cast<uint8>(value); };
};


struct CPU
{
    static void Init();
    /// Установить на PA2 признак того, что процессор занят
    static void SetBusy();
    /// Установить на PA2 признак того, что процессор свободен
    static void SetReady();
    
    static void WritePin(GeneratorWritePin pin, bool set);

private:

    static void InitGPIOS();
};
