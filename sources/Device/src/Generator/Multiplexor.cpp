#include "defines.h"
#include "Multiplexor.h"
#include "AD9952.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    A0_MX1 - PF0
    A1_MX1 - PF1
    A0_MX2 - PF2
    A0_MX2 - PF3
*/

#define PIN_MX1_A0  GPIO_PIN_0
#define PIN_MX2_A0  GPIO_PIN_2

Form Multiplexor::mode[Chan::Number] = {Form::Sine, Form::Sine};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Form Multiplexor::GetMode(Chan ch)
{
    return mode[ch];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Multiplexor::Init()
{
    GPIO_InitTypeDef  isGPIO =
    {
        PIN_MX1_A0 | PIN_MX2_A0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };
    HAL_GPIO_Init(GPIOF, &isGPIO);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Multiplexor::WriteRegister(Register reg, uint value)
{
    uint16 pinA0 = PIN_MX1_A0;

    if(reg == Register::Multiplexor2)
    {
        pinA0 = PIN_MX2_A0;
    }

    if(value & 0x1)  {   SetPin(pinA0); }
    else             { ResetPin(pinA0); }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Multiplexor::SetMode(Chan ch, Form form)
{
    mode[ch] = form;

#define PIN_MX(ch) pins[ch]

    static const uint16 pins[2] =
    {
        PIN_MX1_A0,
        PIN_MX2_A0
    };

    if(form.Is(Form::Sine))
    {
        SetPin(PIN_MX(ch));
    }
    else if(form.Is(Form::PacketImpuls))
    {
        ResetPin(PIN_MX1_A0);
    }
    else                                                // Для всех остальных форм сигнала используется ПЛИС
    {
        ResetPin(PIN_MX(ch));
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
