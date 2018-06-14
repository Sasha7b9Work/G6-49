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

    static void EnableChannel(Channel ch, bool enable);

    static void SetFormWave(Channel ch, WaveForm form);

    static void SetParameter(Channel ch, Type_WaveParameter param, float value);

    static void SetDebugMode(bool enable);

private:
    static void TestSend(uint8 *buffer, int size);

    static void SendToInterface(uint8 *buffer, int size);
    /// Сдвигает буфер на один бит влево
    static void ShiftToLeft(uint8 *buffer, int length);

    static void MasterSynchro();
};

/** @}  @} */
