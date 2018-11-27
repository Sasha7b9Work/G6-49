#pragma once
#include "Settings/SettingsTypes.h"
#include "Command.h"


/** @addtogroup Panel
 *  @{
 *  @defgroup Interface
 *  @{
 */


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define START_MODE(ch)          (set.fpga_startMode[ch])
#define START_MODE_A            START_MODE(Chan::A)
#define START_MODE_B            START_MODE(Chan::B)
#define START_MODE_A_IS_SINGLE  (START_MODE_A == StartMode::Single)
#define START_MODE_B_IS_SINGLE  (START_MODE_B == StartMode::Single)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Generator
{
public:
    static void Reset();

    static void EnableChannel(Chan::E ch, bool enable);

    static void SetFormWave(Wave *w);

    static void SetFormWave(Chan ch, Form::E form);

    static void SetParameter(ParameterValue *param);

    static void SetParameter(ParameterChoice *param);

    static void SetOffset(Chan::E ch, float offset);

    static void SetAmplitude(Chan::E ch, float amplitude);

    static void SetDebugMode(bool enable);

    static void LoadRegister(Register::E reg, uint64 value);

    static void LoadSettings();

    static void TuneChannel(Chan::E ch);
    /// Загружает режим запуска из set в аппаратуру
    static void LoadStartMode(Chan::E ch, int mode);

private:
    static const int FPGA_NUM_POINTS = 8 * 1024;

    static void SetFormDDS(Form *form);
    /// Преобразует данные, записанные в относительных единицах [-1.0f;1.0f] в данные, записанные в прямом коде, пригодные для отправки в ПЛИС
    static void TransformDataToCode(float data[FPGA_NUM_POINTS], uint8 code[FPGA_NUM_POINTS * 2]);
};

/** @}  @} */
