#include "defines.h"
#include "Multiplexor.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    A0_MX1 - PF0
    A1_MX1 - PF1
    A0_MX2 - PF2
    A0_MX2 - PF3
*/

#define PIN_MXA_A0       GPIO_PIN_0
#define PIN_MXA_A1       GPIO_PIN_1
#define PIN_MXB_A0       GPIO_PIN_2
#define PIN_MXB_A1       GPIO_PIN_3


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Multiplexor::Init()
{
    GPIO_InitTypeDef  isGPIO =
    {
        PIN_MXA_A0 | PIN_MXA_A1 | PIN_MXB_A0 | PIN_MXB_A1,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };
    HAL_GPIO_Init(GPIOF, &isGPIO);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Multiplexor::WriteRegister(Name_Register reg, uint value)
{
    if(reg == Multiplexor1)
    {
        if(value & 0x1)  {   SetPin(PIN_MXA_A0);   }
        else             { ResetPin(PIN_MXA_A0); }

        if(value & 0x2)  {   SetPin(PIN_MXA_A1);   }
        else             { ResetPin(PIN_MXA_A1); }
    }
    else if(reg == Multiplexor2)
    {
        if (value & 0x1) {   SetPin(PIN_MXB_A0);   }
        else             { ResetPin(PIN_MXB_A0); }

        if (value & 0x2) {   SetPin(PIN_MXB_A1);   }
        else             { ResetPin(PIN_MXB_A1); }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Multiplexor::SetMode(Channel ch, Type_WaveForm form)
{
#define PIN_MX(ch, num) pins[ch][num]

    static const uint16 pins[2][2] =
    {
        {PIN_MXA_A0, PIN_MXA_A1},
        {PIN_MXB_A0, PIN_MXB_A1}
    };

    if(form == Sine || form == Cosine)
    {
        SetPin(PIN_MX(ch, 0));
        ResetPin(PIN_MX(ch, 1));
    }
    else if(form == Impulse)
    {
        ResetPin(PIN_MX(ch, 0));
        SetPin(PIN_MX(ch, 1));
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
