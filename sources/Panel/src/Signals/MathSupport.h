// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"


struct Value;
struct DParam;
struct IParam;


namespace MathDouble
{
    // ���������� �������� ������������� Value � numDigits ��������� ������� � �������.
    cstr GetIndicatedValue(const DParam *param);

    // ���������� ������� ������� ���������� ������� "1" - ������� (1e1), "0" - ������� (1e0), "-1" - ������� (1e-1), "-2" - ����� (1e-2)
    int GetPositionFirstDigit(const DParam *param, Order::E order = Order::Count);

    int GetPositionFirstDigit(const Value &value, Order::E order);

    // ���������� ����� � ������� position. ����� ��������� �������������� order. One - ����� ������, Kilo - ����� ����� � ��� �����.
    // Order::Count - ��������� �� ��������� - ������� � ������� ������������ ����������� ��������� ��������
    int GetDigit(const Value &value, int position, Order::E order = Order::Count);

    // ���������� ������ � ������� position. ���� �� �����������. ����� ��������� �������������� order. One - ����� ������, Kilo - ����� ����� � ��� �����.
    // Order::Count - ��������� �� ��������� - ������� � ������� ������������ ����������� ��������� ��������
    char GetChar(const Value &value, int postition, Order::E order);
};


namespace MathParameterDouble
{
    void SetParameterDouble(DParam *);

    // ���������� ���������� �������� ��������� ����� ������� (�������� �������� ������). � �������������, ��� �������� ��������� � ������� order
    int GetNumberDigitsBeforeComma(Order::E order = Order::Count);

    // ���������� ���������� �������� ��������� ����� �������
    int GetNumberDigitsAfterComma(Order::E order = Order::Count);
};


namespace MathParameterInteger
{
    // ���������� ���������� �������� ��� ��������� parameter (� ��� ������ ���������� ����������� ��������� �������� ��������� parameter)
    int GetMaxNumberDigits(const IParam *parameter);
};
