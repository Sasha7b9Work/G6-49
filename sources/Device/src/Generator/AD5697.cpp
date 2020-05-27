#include "defines.h"
#include "common/Command.h"
#include "Generator/AD5697.h"
#include "Generator/Calibrator.h"
#include "Generator/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/CalibrationSettings.h"
#include "Utils/Math.h"

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)   // cast truncates constant value
#endif


void AD5697::Init()
{
    Reset(Chan::A);
    Reset(Chan::B);

    HAL_I2C1::Init();
}


double AD5697::CalculateCodeOffset(Chan::E ch)
{
    double zero = Calibrator::GetOffsetK_Zero(ch);           // 2048

    double offset = SettingsGenerator::Offset(ch);

    double max = SettingsGenerator::Amplitude(ch) > 1.0 ? 5.0 : 2.5;

    double result = zero;
    
    if (offset > 0.0F)
    {
        double pos = Calibrator::GetOffsetK_Positive(ch);    // 0

        double scale = (zero - pos) / max;

        double before = pos + scale * (max - offset);

        result = 4095.0 - before;
    }
    else if(offset < 0.0F)
    {
        double neg = Calibrator::GetOffsetK_Negative(ch);    // 4095

        double scale = (neg - zero) / max;

        double before = neg - scale * (max + offset);

        result = 4095.0F - before;
    }

    return Math::Limitation<double>(&result, 0.0, 4095.0);
}


void AD5697::SetOffset(Chan::E ch)
{
    double code = CalculateCodeOffset(ch);

    uint16 value = static_cast<uint16>(static_cast<uint16>(code) << 4);

    uint8 data[3] =
    {
        static_cast<uint8>(BIN_U8(00010000) | (ch == Chan::A ? 0x01 : 0x08)), 
        static_cast<uint8>(value >> 8),
        static_cast<uint8>(value)
    };

    WriteParameter(BIN_U8(00001100), data, WR_AD5697_OFFSET);
}


void AD5697::SetFreqHysteresys(double hyst)
{
    Math::Limitation(&hyst, 0.0, 4095.0);

    uint16 value = static_cast<uint16>(static_cast<uint16>(hyst) << 4);

    uint8 data[3] =
    {
        static_cast<uint8>(BIN_U8(00010000) | 0x08),
        static_cast<uint8>(value >> 8),
        static_cast<uint8>(value)
    };

    WriteParameter(BIN_U8(00001101), data, WR_AD5697_FREQ);
}


void AD5697::SetFreqLevel(double level)
{
    level += Calibrator::GetFreqMeterK_Trig();

    Math::Limitation(&level, 0.0, 4095.0);

    uint16 value = static_cast<uint16>(static_cast<uint16>(level) << 4);

    uint8 data[3] =
    {
        static_cast<uint8>(BIN_U8(00010000) | 0x01),
        static_cast<uint8>(value >> 8),
        static_cast<uint8>(value)
    };

    WriteParameter(BIN_U8(00001101), data, WR_AD5697_FREQ);
}


//uint8 AD5697::CreateCommandByte(ParameterValue param)
//{
//    return (uint8)(BIN_U8(00010000) | (param.Is(ParameterValue::Amplitude) ? 0x01 : 0x08));
//}



void AD5697::WriteParameter(uint8 address, uint8 data[3], HPort::E port, uint16 pin)
{
    HAL_I2C1::Transmit(address, data);
    HAL_PIO::Reset(port, pin);
    HAL_PIO::Set(port, pin);
}


void AD5697::Reset(Chan::E ch)
{
    StructPIN str = PinLDAC(ch);

    HAL_PIO::Set(str.port, str.pin);

    static const StructPIN structs[Chan::Count] = { {WR_AD5697_D_RSA}, {WR_AD5697_D_RSB} };

    HPort::E port = structs[ch].port;
    uint16 pin = structs[ch].pin;

    HAL_PIO::Set(port, pin);
    HAL_PIO::Reset(port, pin);
    HAL_PIO::Set(port, pin);
}


StructPIN AD5697::PinLDAC(Chan::E ch)
{
    static const StructPIN strLDAC[Chan::Count] = { {WR_AD5697_OFFSET}, {WR_AD5697_FREQ} };

    return strLDAC[ch];
}


#ifdef WIN32
#pragma warning(pop)
#endif
