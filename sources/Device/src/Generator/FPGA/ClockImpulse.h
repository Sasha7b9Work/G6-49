// 2022/08/30 11:42:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Common/Common.h"
#include "Common/CommonTypes.h"


namespace FPGA
{
    // ������� ������� ��� ������������ �������� � ������� ���������. ��� ������������ �������� [0 ��� ... 40 ���]
    // ������� ������� 100 ���, ��� ������������ ����� - 1 ���
    namespace ClockImpulse
    {
        enum E
        {
            _100MHz,
            _1MHz
        };

        void Set(E v);
        bool Is1MHz();

        // �������� ��� ������� ��������� ������������ � �������
        int GetDivider();

        void RecalculateImpulseRegistersIfNeed(const Value duration[Chan::Count]);
    };
}
