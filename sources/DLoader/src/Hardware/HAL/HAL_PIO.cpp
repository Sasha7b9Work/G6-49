// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "HAL_PIO.h"
#include <stm32f4xx_hal.h>


const uint16 HPin::_0 = GPIO_PIN_0;
const uint16 HPin::_1 = GPIO_PIN_1;
const uint16 HPin::_2 = GPIO_PIN_2;
const uint16 HPin::_3 = GPIO_PIN_3;
const uint16 HPin::_4 = GPIO_PIN_4;
const uint16 HPin::_5 = GPIO_PIN_5;
const uint16 HPin::_6 = GPIO_PIN_6;
const uint16 HPin::_7 = GPIO_PIN_7;
const uint16 HPin::_8 = GPIO_PIN_8;
const uint16 HPin::_9 = GPIO_PIN_9;
const uint16 HPin::_10 = GPIO_PIN_10;
const uint16 HPin::_11 = GPIO_PIN_11;
const uint16 HPin::_12 = GPIO_PIN_12;
const uint16 HPin::_13 = GPIO_PIN_13;
const uint16 HPin::_14 = GPIO_PIN_14;
const uint16 HPin::_15 = GPIO_PIN_15;


static const uint modes[HMode::Count] =
{
    GPIO_MODE_ANALOG,
    GPIO_MODE_OUTPUT_PP,
    GPIO_MODE_IT_RISING,
    GPIO_MODE_INPUT,
    GPIO_MODE_AF_PP
};

static const uint pulles[HPull::Count] =
{
    GPIO_NOPULL,
    GPIO_PULLDOWN,
    GPIO_PULLUP
};

static const uint speedes[HSpeed::Count] =
{
    GPIO_SPEED_FREQ_LOW,
    GPIO_SPEED_FREQ_HIGH,
    GPIO_SPEED_FREQ_VERY_HIGH
};

static const uint alternates[HAlternate::Count] =
{
    GPIO_AF0_MCO,
    GPIO_AF5_SPI1,
    GPIO_AF7_USART3,
    GPIO_AF10_OTG_FS,
    GPIO_AF12_OTG_HS_FS
};


static GPIO_TypeDef * const ports[HPort::Count] =
{
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    GPIOH
};

#define PORT(p)      (ports[p])
#define MODE(m)      (modes[m])
#define PULL(p)      (pulles[p])
#define SPEED(s)     (speedes[s])
#define ALTERNATE(a) (alternates[a])



void HAL_PIO::Init(HPort::E port, uint pins, HMode::E mode, HPull::E pull, HSpeed::E speed, HAlternate::E alternate)
{
    GPIO_InitTypeDef isGPIO =
    {
        pins,
        MODE(mode),
        PULL(pull),
        SPEED(speed),
        ALTERNATE(alternate)
    };

    HAL_GPIO_Init(PORT(port), &isGPIO);
}


void HAL_PIO::Set(HPort::E port, uint16 pin)
{
    HAL_GPIO_WritePin(PORT(port), pin, GPIO_PIN_SET);
}


void HAL_PIO::Set(const StructPIN &pin)
{
    HAL_GPIO_WritePin(PORT(pin.port), pin.pin, GPIO_PIN_SET);
}


void HAL_PIO::Reset(HPort::E port, uint16 pin)
{
    HAL_GPIO_WritePin(PORT(port), pin, GPIO_PIN_RESET);
}


void HAL_PIO::Reset(const StructPIN &pin)
{
    HAL_GPIO_WritePin(PORT(pin.port), pin.pin, GPIO_PIN_RESET);
}


void HAL_PIO::Write(HPort::E port, uint16 pin, HState::E state)
{
    HAL_GPIO_WritePin(PORT(port), pin, (state == HState::Set) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


void HAL_PIO::Write(HPort::E port, uint16 pin, bool state)
{
    HAL_GPIO_WritePin(PORT(port), pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


uint8 HAL_PIO::Read(HPort::E port, uint16 pin)
{
    return static_cast<uint8>(HAL_GPIO_ReadPin(PORT(port), pin));
}


void HAL_PIO::Init()
{
    StructPIN writePins[] =
    {
        {WR_AD9952_SPI3_CSA  },
        {WR_AD9952_SPI3_CSB  },
        {WR_AD9952_IO_UPD    },
        {WR_AD9952_IOSYNA    },
        {WR_AD9952_IOSYNB    },
        {WR_AD9952_RES_DDS   },
        {WR_P1_AMPL_A        },
        {WR_P2_AMPL_B        },
        {WR_OUT_A            },
        {WR_OUT_B            },
        {WR_AD5697_OFFSET    },
        {WR_AD5697_FREQ      },
        {WR_AD5697_D_RSA     },
        {WR_AD5697_D_RSB     },
        {WR_FPGA_WR_RG       },
        {WR_FPGA_CLK_RG      },
        {WR_FPGA_DT_RG       },
        {WR_FPGA_A0_RG       },
        {WR_FPGA_A1_RG       },
        {WR_FPGA_A2_RG       },
        {WR_FPGA_A3_RG       },
        {WR_FPGA_WR_DATA     },
        {WR_FREQ_METER_RESIST},
        {WR_FREQ_METER_COUPLE},
        {WR_FREQ_METER_FILTR },
        {WR_FREQ_METER_CLK   },
        {WR_CPU_BUSY         },
        {HPort::Count, 0}
    };

    StructPIN *str = writePins;

    while(str->port != HPort::Count)
    {
        Init(str->port, str->pin, HMode::Output_PP, HPull::No, HSpeed::High);
        Reset(str->port, str->pin);

        str++;
    }

    StructPIN readPins[] =
    {
        {RD_FREQ_METER_DRY },
        {RD_FREQ_METER_DATA},
        {HPort::Count, 0}
    };

    str = readPins;

    while(str->port != HPort::Count)
    {
        Init(str->port, str->pin, HMode::Input, HPull::Down);
        
        str++;
    }
}


void HAL_PIO::WriteFPGA(uint8 byte)
{
    //                                                                                 биты 0,1                                    биты 2,3
    GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<int16>(byte) & 0x03) << 14) + ((static_cast<uint16>(byte & 0x0c)) >> 2);;
    //                                                                                биты 4,5,6
    GPIOE->ODR = (GPIOE->ODR & 0xf87f) + static_cast<uint16>((static_cast<int16>(byte) & 0xf0) << 3);
}
