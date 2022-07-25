// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"


struct Value;
class ParameterDouble;
class ParameterInteger;


namespace MathDouble
{
    // ���������� �������� ������������� Value � numDigits ��������� ������� � �������.
    pString GetIndicatedValue(const ParameterDouble *param);

    // ���������� ������� ������� ���������� ������� "1" - ������� (1e1), "0" - ������� (1e0), "-1" - ������� (1e-1), "-2" - ����� (1e-2)
    int GetPositionFirstDigit(const ParameterDouble *param, Order::E order = Order::Count);

    int GetPositionFirstDigit(const Value &value, Order::E order);

    // ���������� ����� � ������� position. ����� ��������� �������������� order. One - ����� ������, Kilo - ����� ����� � ��� �����.
    // Order::Count - ��������� �� ��������� - ������� � ������� ������������ ����������� ��������� ��������
    int GetDigit(const Value &value, int position, Order::E order = Order::Count);

    // ���������� ������ � ������� position. ���� �� �����������. ����� ��������� �������������� order. One - ����� ������, Kilo - ����� ����� � ��� �����.
    // Order::Count - ��������� �� ��������� - ������� � ������� ������������ ����������� ��������� ��������
    char GetChar(const Value &value, int postition, Order::E order);
};


struct MathParameterDouble
{
    static void SetParameterDouble(ParameterDouble *parameter) { param = parameter; };

    // ���������� ���������� �������� ��������� ����� ������� (�������� �������� ������). � �������������, ��� �������� ��������� � ������� order
    static int GetNumberDigitsBeforeComma(Order::E order = Order::Count);

    // ���������� ���������� �������� ��������� ����� �������
    static int GetNumberDigitsAfterComma(Order::E order = Order::Count);

private:

    // ���������� ��������, �� �������� ����� ������������ ����������
    static ParameterDouble *param;
};


namespace MathParameterInteger
{
    // ���������� ���������� �������� ��� ��������� parameter (� ��� ������ ���������� ����������� ��������� �������� ��������� parameter)
    int GetMaxNumberDigits(const ParameterInteger *parameter);
};
