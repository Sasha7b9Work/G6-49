#include "defines.h"
#include "AD5697.h"
#include "Command.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static I2C_HandleTypeDef hI2C =
{
    I2C1,
    {
        100000,                     // ClockSpeed
        I2C_DUTYCYCLE_2,            // DutyCycle
        0,                          // OwnAddress1
        I2C_ADDRESSINGMODE_7BIT,    // AddressingMode
        I2C_DUALADDRESS_DISABLE,    // DualAddressMode
        0,                          // OwnAddress2
        I2C_GENERALCALL_DISABLE,    // GeneralCallMode
        I2C_NOSTRETCH_DISABLE       // NoStretchMode
    },
    0, 0, 0, 0, 0, 0, 0, HAL_UNLOCKED, HAL_I2C_STATE_RESET, HAL_I2C_MODE_NONE, 0, 0, 0, 0, 0
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AD5697::Init()
{
    Reset(Chan::A);
    Reset(Chan::B);
    
    GPIO_InitTypeDef isGPIO =
    {//    SCL          SDA
        GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_AF_OD,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF4_I2C1
    };
    HAL_GPIO_Init(GPIOB, &isGPIO);

    HAL_I2C_Init(&hI2C);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD5697::SetOffset(Chan ch, float offset)
{   
    Limitation(&offset, 0.0f, 4095.0f);

    uint16 value = (uint16)((uint16)offset << 4);

    uint8 data[3] = {(uint8)(BINARY_U8(00010000) | (ch.IsA() ? 0x01 : 0x08)), (uint8)(value >> 8), (uint8)value};

    WriteParameter(BINARY_U8(00001100), data, GeneratorWritePin::AD5697_Offset);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD5697::SetFreqHysteresys(float hyst)
{
    Limitation(&hyst, 0.0f, 4095.0f);

    uint16 value = (uint16)((uint16)hyst << 4);

    uint8 data[3] = {((uint8)(BINARY_U8(00010000) | 0x08)), (uint8)(value >> 8), (uint8)value};

    WriteParameter(BINARY_U8(00001101), data, GeneratorWritePin::AD5697_Freq);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD5697::SetFreqLevel(float level)
{
    Limitation(&level, 0.0f, 4095.0f);

    uint16 value = (uint16)((uint16)level << 4);

    uint8 data[3] = {((uint8)(BINARY_U8(00010000) | 0x01)), (uint8)(value >> 8), (uint8)value};

    WriteParameter(BINARY_U8(00001101), data, GeneratorWritePin::AD5697_Freq);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 AD5697::CreateCommandByte(WaveParameter param)
{
    return (uint8)(BINARY_U8(00010000) | (param.Is(WaveParameter::Amplitude) ? 0x01 : 0x08));
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD5697::WriteParameter(uint8 address, uint8 data[3], GeneratorWritePin pin)
{
    TransmitI2C(address, data);
    CPU::WritePin(pin, false);
    CPU::WritePin(pin, true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD5697::TransmitI2C(uint8 address, uint8 data[3])
{
    // Смещение на один бит влево - страшная штука. Если не знать, можно потерять много времени
    HAL_I2C_Master_Transmit(&hI2C, (uint16)(address << 1), data, 3, 100);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD5697::Reset(Chan ch)
{
    static const GeneratorWritePin pinRS[Chan::Number] = {GeneratorWritePin::AD5697_D_RSA, GeneratorWritePin::AD5697_D_RSB};

    CPU::WritePin(PinLDAC(ch), true);
    CPU::WritePin(pinRS[ch], true);
    CPU::WritePin(pinRS[ch], false);
    CPU::WritePin(pinRS[ch], true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
GeneratorWritePin AD5697::PinLDAC(Chan ch)
{
    static const GeneratorWritePin pinLDAC[Chan::Number] = {GeneratorWritePin::AD5697_Offset, GeneratorWritePin::AD5697_Freq};

    return pinLDAC[ch];
}
