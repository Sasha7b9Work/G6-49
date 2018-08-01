#pragma once


enum Channel
{
    A,
    B,
    NumChannels
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
