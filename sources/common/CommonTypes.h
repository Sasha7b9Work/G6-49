#pragma once
#include "defines.h"


struct Chan
{
    enum E
    {
        A,
        B,
        Count
    } value;
    explicit Chan(uint8 v) : value(static_cast<E>(v)) {};
    explicit Chan(E v) : value(v)   {};
    bool IsA() const                { return value == A; };
    bool IsB() const                { return value == B; };
    bool Is(const Chan &ch) const   { return value == ch.value; }
    operator uint8()        const   { return static_cast<uint8>(value); }
    operator int()          const   { return static_cast<int>(value); }
    Chan GetInverse()               { return (value == A) ? Chan(B) : Chan(A); }
    bool Enabled() const;
};


extern Chan ChA;
extern Chan ChB;


// Сопротивление входа частотомера
struct FreqResist
{
    enum E
    {
        _1MOhm,
        _50Ohm
    } value;
    bool Is(E v) const { return value == v; };
    FreqResist(E v) : value(v) {};
    FreqResist(uint v) : value(static_cast<E>(v)) {};
    operator uint() const { return static_cast<uint>(value); };
};

// Открытый/закрытый вход частотомера
struct FreqCouple
{
    enum E
    {
        AC,
        DC
    } value;
    FreqCouple(E v) : value(v) {};
    FreqCouple(uint v) : value(static_cast<E>(v)) {};
    bool Is(E v) const { return v == value; };
    operator uint() const { return static_cast<uint>(value); };
};

// ФНЧ частотомера
struct FreqFiltr
{
    enum E
    {
        Disable,
        Enable
    } value;
    FreqFiltr(E v) : value(v) {};
    FreqFiltr(uint v) : value(static_cast<E>(v)) {};
    bool Is(E v) const { return v == value; };
    operator uint() const { return static_cast<uint>(value); };
};

// Режим запуска
struct StartMode
{
    enum E
    {
        Auto,           ///< Автоматический
        Single,         ///< Однократный запуск по нажатию кнопки
        ComparatorA,    ///< Компаратор канала A (в момент перехода из нуля в плюс выхода AD9952 канала A
        ShaperB         ///< Формирователь импульсов канала B (режим "Импульсный сигнал")
    } value;

    StartMode(float v) : value(static_cast<E>(static_cast<int>(v + 0.5F))) {};
    operator uint8() const { return static_cast<uint8>(value); };
    bool Is(StartMode start) const { return start.value == value; };
};

struct KoeffCal
{
    enum E
    {
        AD9952_NEG,         ///< Это значение определяет код, засылаемый для смещения -5В синусоидального сигнала
        AD9952_ZERO,        ///< Это значение определяет код, засылаемый для смещения 0В синусоидального сигнала
        AD9952_POS,         ///< Это значение определяет код, засылаемый для смещения +5В синусоидального сигнала
        AD9952_AMPL,        ///< Сотые доли процентов поправочного коэффициента к амплитуде синусоидального сигнала : ЗАСЫЛАЕМЫЙ_КОД = РАССЧИТАННЫЙ_КОД * (1 + AD9952_AMPL / 10000)
        DDS_MAX,            ///< Код, соответствующий уровню +5В DDS
        DDS_MIN,            ///< Код, соответствующий уровню -5В DDS
        DDS_OFFSET,         ///< Код, соотвествующий нулевому уровню смещения DDS
        FREQ_LEVEL_TRIG,    ///< Калибровочный коэффициент уровня синхронизации частотомера
        Count
    } value;
};
