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


// ������������� ����� �����������
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

// ��������/�������� ���� �����������
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

// ��� �����������
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

// ����� �������
struct StartMode
{
    enum E
    {
        Auto,           ///< ��������������
        Single,         ///< ����������� ������ �� ������� ������
        ComparatorA,    ///< ���������� ������ A (� ������ �������� �� ���� � ���� ������ AD9952 ������ A
        ShaperB         ///< ������������� ��������� ������ B (����� "���������� ������")
    } value;

    StartMode(float v) : value(static_cast<E>(static_cast<int>(v + 0.5F))) {};
    operator uint8() const { return static_cast<uint8>(value); };
    bool Is(StartMode start) const { return start.value == value; };
};

struct KoeffCal
{
    enum E
    {
        AD9952_NEG,         ///< ��� �������� ���������� ���, ���������� ��� �������� -5� ��������������� �������
        AD9952_ZERO,        ///< ��� �������� ���������� ���, ���������� ��� �������� 0� ��������������� �������
        AD9952_POS,         ///< ��� �������� ���������� ���, ���������� ��� �������� +5� ��������������� �������
        AD9952_AMPL,        ///< ����� ���� ��������� ������������ ������������ � ��������� ��������������� ������� : ����������_��� = ������������_��� * (1 + AD9952_AMPL / 10000)
        DDS_MAX,            ///< ���, ��������������� ������ +5� DDS
        DDS_MIN,            ///< ���, ��������������� ������ -5� DDS
        DDS_OFFSET,         ///< ���, �������������� �������� ������ �������� DDS
        FREQ_LEVEL_TRIG,    ///< ������������� ����������� ������ ������������� �����������
        Count
    } value;
};
