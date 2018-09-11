#include "defines.h"
#include "Multiplexor.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    A0_MX1 - PF0
    A1_MX1 - PF1
    A0_MX2 - PF2
    A0_MX2 - PF3
*/

#define PIN_MX1_A0  GPIO_PIN_0
#define PIN_MX1_A1  GPIO_PIN_1
#define PIN_MX2_A0  GPIO_PIN_2
#define PIN_MX2_A1  GPIO_PIN_3
#define PIN_MX3_A0  GPIO_PIN_4
#define PIN_MX3_A1  GPIO_PIN_5


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Multiplexor::Init()
{
    GPIO_InitTypeDef  isGPIO =
    {
        PIN_MX1_A0 | PIN_MX1_A1 | PIN_MX2_A0 | PIN_MX2_A1 | PIN_MX3_A0 | PIN_MX3_A1,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };
    HAL_GPIO_Init(GPIOF, &isGPIO);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Multiplexor::WriteRegister(Register reg, uint value)
{
    uint16 pinA0 = PIN_MX1_A0;
    uint16 pinA1 = PIN_MX1_A1;

    if(reg == Register::Multiplexor2)
    {
        pinA0 = PIN_MX2_A0;
        pinA1 = PIN_MX2_A1;
    }
    else if(reg == Register::Multiplexor3)
    {
        pinA0 = PIN_MX3_A0;
        pinA1 = PIN_MX3_A1;
    }

    if(value & 0x1)  {   SetPin(pinA0); }
    else             { ResetPin(pinA0); }
    
    if(value & 0x2)  {   SetPin(pinA1); }
    else             { ResetPin(pinA1); }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Multiplexor::SetMode(Chan ch, Form form)
{
#define PIN_MX(ch, num) pins[ch][num]

    static const uint16 pins[2][2] =
    {
        {PIN_MX1_A0, PIN_MX1_A1},
        {PIN_MX2_A0, PIN_MX2_A1}
    };

    if(form == Form::Sine)
    {
        SetPin(PIN_MX(ch, 0));
        ResetPin(PIN_MX(ch, 1));
    }
    else                                                // Для всех остальных форм сигнала используется ПЛИС
    {
        ResetPin(PIN_MX(ch, 0));
        ResetPin(PIN_MX(ch, 1));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Multiplexor::SetPin(uint16 pin)
{
    HAL_GPIO_WritePin(GPIOF, pin, GPIO_PIN_SET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Multiplexor::ResetPin(uint16 pin)
{
    HAL_GPIO_WritePin(GPIOF, pin, GPIO_PIN_RESET);
}
