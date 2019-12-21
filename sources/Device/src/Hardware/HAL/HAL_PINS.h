#pragma once


/// Пины записи
#define WR_AD9952_SPI3_CSA      HPort::_F, HPin::_6
#define WR_AD9952_SPI3_CSB      HPort::_F, HPin::_7
#define WR_AD9952_IO_UPD        HPort::_C, HPin::_7
#define WR_AD9952_IOSYNA        HPort::_F, HPin::_8
#define WR_AD9952_IOSYNB        HPort::_F, HPin::_9
#define WR_AD9952_RES_DDS       HPort::_C, HPin::_8
#define WR_P1_AMPL_A            HPort::_E, HPin::_12
#define WR_P2_AMPL_B            HPort::_E, HPin::_13
#define WR_OUT_A                HPort::_F, HPin::_1
#define WR_OUT_B                HPort::_C, HPin::_15
#define WR_AD5697_OFFSET        HPort::_C, HPin::_2     ///< Управление смещением выхода AD9952
#define WR_AD5697_FREQ          HPort::_C, HPin::_3     ///< Управление параметрами частотомера
#define WR_AD5697_D_RSA         HPort::_C, HPin::_5
#define WR_AD5697_D_RSB         HPort::_B, HPin::_0
#define WR_FPGA_WR_RG           HPort::_D, HPin::_11
#define WR_FPGA_CLK_RG          HPort::_D, HPin::_12
#define WR_FPGA_DT_RG           HPort::_D, HPin::_13
#define WR_FPGA_A0_RG           HPort::_G, HPin::_5
#define WR_FPGA_A1_RG           HPort::_G, HPin::_6
#define WR_FPGA_A2_RG           HPort::_G, HPin::_7
#define WR_FPGA_A3_RG           HPort::_G, HPin::_8
#define WR_FPGA_WR_DATA         HPort::_D, HPin::_5
#define WR_FREQ_METER_RESIST    HPort::_G, HPin::_12
#define WR_FREQ_METER_COUPLE    HPort::_G, HPin::_13
#define WR_FREQ_METER_FILTR     HPort::_G, HPin::_14
#define WR_FREQ_METER_CLK       HPort::_C, HPin::_1     ///< Пин формирования тактов чтения измеренного значения


// Пины чтения
#define RD_FREQ_METER_DRY       HPort::_C, HPin::_0     ///< Пин чтения готовности частотомера к выдаче данных
#define RD_FREQ_METER_DATA      HPort::_B, HPin::_12    ///< Пин чтения данных
