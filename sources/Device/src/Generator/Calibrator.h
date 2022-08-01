// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "common/CommonTypes.h"


namespace Calibrator
{
    // ������������� ����������� ����������, ������������ ������������ ������������ ��� ����� ������������ ���������
    // param : 0 - ���������
    //         1 - +2.5� / +5�
    //         2 - 0�
    //         3 - -2.5� / -5�
    void SetK(uint8 channel, uint8 signal, uint8 range, uint8 param, int16 k);
    
    // ���������� ����������� ���������� ���������
    float GetAmplitudeK(const Chan &);
    
    // ���������� ������������� ����������� �������� ��� 0�
    float GetOffsetK_Zero(const Chan &);
    
    // ���������� ������������� ����������� �������� ��� -5�
    double GetOffsetK_Negative(const Chan &);
    
    // ���������� ������������� ����������� �������� ��� +5�
    double GetOffsetK_Positive(const Chan &);
    
    // ��������� �������������� ������������ ������ ������������� �����������
    void SetFreqMeterK_Trig(int16 k);

    // ���������� ������������� ����������� ������ ������������� �����������
    float GetFreqMeterK_Trig();
};
