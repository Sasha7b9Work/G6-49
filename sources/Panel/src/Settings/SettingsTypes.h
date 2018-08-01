#pragma once
#include "Command.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum Channel
{
    A,
    B,
    NumChannels
};

enum BillingTime
{
    BillingTime_1ms,
    BillingTime_10ms,
    BillingTime_100ms,
    BillingTime_1000ms,
    BillingTime_10000ms
};

enum Language
{
    RU,
    EN
};

enum SizeByte
{
    Size_7bit,
    Size_8bit
};

enum StopBit
{
    StopBit_1,
    StopBit_2
};

enum Parity
{
    Parity_Off,
    Parity_On
};

/// 
enum FreqInterval
{
    FreqInterval_1,
    FreqInterval_10
};

/// Сопротивление входа частотомера
enum FreqResist
{
    FreqResist_1MOhm,
    FreqResist_50Ohm
};

/// Открытый/закрытый вход частотомера
enum FreqCouple
{
    FreqCouple_AC,
    FreqCouple_DC
};

/// ФНЧ частотомера
enum FreqFiltr
{
    FreqFiltr_Enable,
    FreqFiltr_Disable
};


float MinValue(Type_WaveParameter param);
float MaxValue(Type_WaveParameter param);
