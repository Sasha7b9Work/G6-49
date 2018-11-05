#pragma once
#include "Settings/SettingsTypes.h"
#include "Command.h"
#include "Utils/Buffer.h"


/** @addtogroup Panel
 *  @{
 *  @defgroup Interface
 *  @{
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define START_MODE(ch)          (set.fpga_startMode[ch])
#define START_MODE_A            START_MODE(Chan::A)
#define START_MODE_B            START_MODE(Chan::B)
#define START_MODE_A_IS_SINGLE  (START_MODE_A == StartMode::Single)
#define START_MODE_B_IS_SINGLE  (START_MODE_B == StartMode::Single)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Generator
{
public:
    static void Reset();
    /// Эта функция вызывается в цилке для приёма сообщений от прибора
    static void Update();

    static void EnableChannel(Chan ch, bool enable);

    static void SetFormWave(Wave *w);

    static void SetParameter(ParameterValue *param);

    static void SetParameter(ParameterChoice *param);

    static void SetDebugMode(bool enable);

    static void LoadRegister(Register reg, uint64 value);

    static void TuneChannel(Chan ch);
    /// Загружает режим запуска из set в аппаратуру
    static void LoadStartMode(Chan ch, int mode);

private:
    static const int FPGA_NUM_POINTS = 8 * 1024;
    /// Заслать в генератор данные
    static void SendToInterface(const uint8 *buffer, uint16 size);

    static void SendToInterface(const Buffer &buffer);
    /// Принять и обработать данные от ПЛИС
    static void ProcessDataFPGA();
    /// Принять numBytes байт от ПЛИС и выполнить их
    static void ReceiveAndRun(uint16 numBytes);
    
    static void SetFormDDS(Form *form);
    /// Загружает точки произвольного сигнала в ПЛИС
    static void LoadPointsToDDS(Chan ch, uint8 points[FPGA_NUM_POINTS * 2]);
    /// Преобразует данные, записанные в относительных единицах [-1.0f;1.0f] в данные, записанные в прямом коде, пригодные для отправки в ПЛИС
    static void TransformDataToCode(float data[FPGA_NUM_POINTS], uint8 code[FPGA_NUM_POINTS * 2]);
};

/** @}  @} */
