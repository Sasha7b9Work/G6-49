#pragma once
#include "Settings/SettingsTypes.h"
#include "Command.h"


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

    static bool ChannelEnabled(Chan ch);

    static void SetFormWave(Wave *w);

    static void SetParameter(ParameterValue *param);

    static void SetParameter(ParameterChoice *param);

    static void SetDebugMode(bool enable);

    static void LoadRegister(Register reg, uint64 value);

    static void TuneChannel(Chan ch);

    static void EmptyCommand();
    /// Загружает режим запуска из set в аппаратуру
    static void LoadStartMode(Chan ch);
    static void LoadStartMode(Chan ch, int mode);

private:
    /// Заслать в генератор данные
    static void SendToInterface(uint8 *buffer, uint16 size);

    static void SendToInterfaceNew(uint8 *buffer, uint16 size);

    static bool enabledCh[Chan::Number];
};

/** @}  @} */
