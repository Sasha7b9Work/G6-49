#pragma once


/*
* ����� ������� ������ �������� ����������, ������� �������������
*/


static uint8 range_null = 0;

DEF_CHOICE_2(cRangeNull,
    "��������", "Range",
    "", "",
    "", "", "", "",
    "", "", "", "",
    range_null, pCalibration, Item::EFuncActive, Choice::EFuncChanged, Item::EFuncDraw
)


static uint8 parameter_null = 0;

DEF_CHOICE_2(cParameterNull,
    "��������", "Param",
    "", "",
    "", "", "", "",
    "", "", "", "",
    parameter_null, pCalibration, Item::EFuncActive, Choice::EFuncChanged, Item::EFuncDraw
)

