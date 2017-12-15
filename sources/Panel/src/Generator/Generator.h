#pragma once
#include "Settings/SettingsTypes.h"


/** @addtogroup Panel
 *  @{
 *  @defgroup Interface
 *  @{
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Generator_Init();

void Generator_Reset();

void Generator_EnableChannel(Channel ch, bool enable);

void Generator_SetFormWave(Channel ch, WaveForm form);

void Generator_SetParameter(Channel ch, WaveParameter param, float value);

void Generator_SetDebugMode(bool enable);

/** @}  @} */
