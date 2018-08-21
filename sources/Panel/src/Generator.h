#pragma once
#include "Settings/SettingsTypes.h"


/** @addtogroup Panel
 *  @{
 *  @defgroup Interface
 *  @{
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Generator
{
public:
    static void Reset();
    /// Эта функция вызывается в цилке для приёма сообщений от прибора
    static void Update();

    static void EnableChannel(Channel ch, bool enable);

    static void SetFormWave(Channel ch, WaveForm form);

    static void SetParameter(Channel ch, WaveParameter param, float value);

    static void SetDebugMode(bool enable);

    static void LoadRegister(Register reg, uint value);

private:
    /// Заслать в генератор данные
    static void SendToInterface(uint8 *buffer, int size);
    /// Читать и выполнять данные из генератора
    static void ReadAndRunFromInterface();

    static void MasterSynchro();

    static void ExecuteCommand(uint8 *buffer, int size);
};

/** @}  @} */
