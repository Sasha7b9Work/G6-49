#include "defines.h"
#include "log.h"
#include "AD5697.h"
#include "Command.h"
#include "FPGA.h"
#include "Settings/CalibrationSettings.h"
#include "Utils/Math.h"

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)   // cast truncates constant value
#endif


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


static float CalculateOffset(Chan::E ch, ParamValue offset)
{
    if (FPGA::CurrentMode(ch) != FPGA::ModeWork::Sine)
    {
        return CAL_DDS_OFFSET(ch);
    }

    float zero = CAL_AD9952_OFFSET_ZERO(ch);        // 2048
    
    if (offset.ToFloat() > 0.0F)
    {
        float pos = CAL_AD9952_OFFSET_POS(ch);      // 0

        float scale = (zero - pos) / 5.0F;

        return pos + scale * (5.0F - offset.ToFloat());
    }
    else if(offset.ToFloat() < 0.0F)
    {
        float neg = CAL_AD9952_OFFSET_NEG(ch);      // 4095

        float scale = (neg - zero) / 5.0F;

        return neg - scale * (5.0F + offset.ToFloat());
    }
    else
    {
        // здесь ничего
    }
    return zero;
}


void AD5697::SetOffset(Chan::E ch, ParamValue offset)
{
    float code = CalculateOffset(ch, offset);

    uint16 value = static_cast<uint16>(static_cast<uint16>(code) << 4);

    uint8 data[3] =
    {
        static_cast<uint8>(BIN_U8(00010000) | (ch == Chan::A ? 0x01 : 0x08)), 
        static_cast<uint8>(value >> 8),
        static_cast<uint8>(value)
    };

    WriteParameter(BIN_U8(00001100), data, GeneratorWritePin::AD5697_Offset);
}


void AD5697::SetFreqHysteresys(float hyst)
{
    Limitation(&hyst, 0.0F, 4095.0F);

    uint16 value = static_cast<uint16>(static_cast<uint16>(hyst) << 4);

    uint8 data[3] =
    {
        static_cast<uint8>(BIN_U8(00010000) | 0x08),
        static_cast<uint8>(value >> 8),
        static_cast<uint8>(value)
    };

    WriteParameter(BIN_U8(00001101), data, GeneratorWritePin::AD5697_Freq);
}


void AD5697::SetFreqLevel(float level)
{
    level += (float)CAL_FREQ_LEVEL_TRIG;

    Limitation(&level, 0.0F, 4095.0F);

    uint16 value = static_cast<uint16>(static_cast<uint16>(level) << 4);

    uint8 data[3] =
    {
        static_cast<uint8>(BIN_U8(00010000) | 0x01),
        static_cast<uint8>(value >> 8),
        static_cast<uint8>(value)
    };

    WriteParameter(BIN_U8(00001101), data, GeneratorWritePin::AD5697_Freq);
}


uint8 AD5697::CreateCommandByte(ParameterValue param)
{
    return (uint8)(BIN_U8(00010000) | (param.Is(ParameterValue::Amplitude) ? 0x01 : 0x08));
}



void AD5697::WriteParameter(uint8 address, uint8 data[3], GeneratorWritePin pin)
{
    TransmitI2C(address, data);
    CPU::WritePin(pin, false);
    CPU::WritePin(pin, true);
}


void AD5697::TransmitI2C(uint8 address, uint8 data[3])
{
    // Смещение на один бит влево - страшная штука. Если не знать, можно потерять много времени
    HAL_I2C_Master_Transmit(&hI2C, static_cast<uint16>(address << 1), data, 3, 100);
}


void AD5697::Reset(Chan::E ch)
{
    static const GeneratorWritePin pinRS[Chan::Count] = {GeneratorWritePin::AD5697_D_RSA, GeneratorWritePin::AD5697_D_RSB};

    CPU::WritePin(PinLDAC(ch), true);
    CPU::WritePin(pinRS[ch], true);
    CPU::WritePin(pinRS[ch], false);
    CPU::WritePin(pinRS[ch], true);
}


GeneratorWritePin AD5697::PinLDAC(Chan::E ch)
{
    static const GeneratorWritePin pinLDAC[Chan::Count] = {GeneratorWritePin::AD5697_Offset, GeneratorWritePin::AD5697_Freq};

    return pinLDAC[ch];
}


#ifdef WIN32
#pragma warning(pop)
#endif
