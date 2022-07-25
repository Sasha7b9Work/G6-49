// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"


struct Value;
class ParameterDouble;
class ParameterInteger;


struct MathDouble
{
    // ���������� �������� ������������� Value � numDigits ��������� ������� � �������.
    static pString GetIndicatedValue(const ParameterDouble *param);

    // ���������� ������� ������� ���������� ������� "1" - ������� (1e1), "0" - ������� (1e0), "-1" - ������� (1e-1), "-2" - ����� (1e-2)
    static int GetPositionFirstDigit(const ParameterDouble *param, Order::E order = Order::Count);

    static int GetPositionFirstDigit(const Value &value, Order::E order);

    // ���������� ����� � ������� position. ����� ��������� �������������� order. One - ����� ������, Kilo - ����� ����� � ��� �����.
    // Order::Count - ��������� �� ��������� - ������� � ������� ������������ ����������� ��������� ��������
    static int GetDigit(const Value &value, int position, Order::E order = Order::Count);

    // ���������� ������ � ������� position. ���� �� �����������. ����� ��������� �������������� order. One - ����� ������, Kilo - ����� ����� � ��� �����.
    // Order::Count - ��������� �� ��������� - ������� � ������� ������������ ����������� ��������� ��������
    static char GetChar(const Value &value, int postition, Order::E order);
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


struct MathParameterInteger
{
    // ���������� ���������� �������� ��� ��������� parameter (� ��� ������ ���������� ����������� ��������� �������� ��������� parameter)
    static int GetMaxNumberDigits(const ParameterInteger *parameter);
};
