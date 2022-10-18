#pragma once


/*
* «десь описаны пустые элементы управлени€, которые подставл€ютс€
*/


static uint8 range_null = 0;

DEF_CHOICE_2(cRangeNull,
    "ƒиапазон", "Range",
    "", "",
    "", "", "", "",
    "", "", "", "",
    range_null, pCalibration, Item::EFuncActive, Choice::EFuncChanged, Item::EFuncDraw
)


static uint8 parameter_null = 0;

DEF_CHOICE_2(cParameterNull,
    "ѕараметр", "Param",
    "", "",
    "", "", "", "",
    "", "", "", "",
    parameter_null, pCalibration, Item::EFuncActive, Choice::EFuncChanged, Item::EFuncDraw
)

