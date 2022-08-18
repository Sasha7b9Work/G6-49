// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace PFreqMeter
{
    // ��������� ��������� � ����������
    void LoadSettings();

    void Draw();
    
    // ���������� ��������� �������� �������
    void SetMeasure(uint value);
    
    // ���� ����� �������� ��� ����, ����� �������, ��� ��������� ���������� ���������.
    void SetInactive();
    
    // ������� � ���� �������� ������
    void LoadLevel();

    // ������� � ���� �������� �����������
    void LoadHysteresis();

    // ������� ���� ��������� � SCPI
    void SendMeasureToSCPI();

    struct BillingTime
    {
        enum E
        {
            _1ms,
            _10ms,
            _100ms,
            _1000ms,
            _10000ms
        } value;

        BillingTime(E v) : value(v) { }

        operator uint8() const { return (uint8)value; };
    };

    // ����� ����������� ������ � ������ ��������� ������� �������
    struct AvePeriod
    {
        enum E
        {
            _1,
            _10,
            _100,
            _1000,
            _10000
        } value;

        AvePeriod(E v) : value(v) { }

        operator uint8() const { return (uint8)value; };
    };
};
