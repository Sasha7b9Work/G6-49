#pragma once
#include "stm32f4xx_hal.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern LTDC_HandleTypeDef hLTDC;
extern uint activeLayer;
#define MAX_LAYER_NUMBER 2


typedef struct
{
    uint textColor;
    uint backColor;
    //sFONT *pFont;
} LCD_DrawPropTypeDef;
