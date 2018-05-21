#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum PanelControl
{
    B_None    = 0x00,
    B_F1            = 0x01,
    B_F2            = 0x02,
    B_F3            = 0x03,
    B_F4            = 0x04,
    B_F5            = 0x05,
    B_0             = 0x06,
    B_1             = 0x07,
    B_2             = 0x08,
    B_3             = 0x09,
    B_4             = 0x0a,
    B_5             = 0x0b,
    B_6             = 0x0c,
    B_7             = 0x0d,
    B_8             = 0x0e,
    B_9             = 0x0f,
    B_Dot           = 0x10,
    B_Minus         = 0x11,
    B_ESC           = 0x12,
    B_LEFT          = 0x13,
    B_RIGHT         = 0x14,
    B_Ch            = 0x15,
    B_WF            = 0x16,
    B_M             = 0x17,
    B_U             = 0x18,
    B_ON1           = 0x19,
    B_ON2           = 0x1a,
    REG_LEFT        = 0x1b,
    REG_RIGHT       = 0x1c,
    REG_BTN
};

enum TypePress
{
    Down,
    Long,
    Up
};

/// Хранит код органа управления и его состояние - если pressed == true, то кнопка находится в нажатом состоянии
struct StructControl
{
    PanelControl key;
    TypePress    typePress;
};


const char *PanelControlName(PanelControl control);
