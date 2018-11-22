#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "structs.h"
#include "CalibrationSettings.h"
#include "Hardware/Modules/EEPROM.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const CalibrationSettings defSet =
{
    0,              // Это значение не должно равняться (-1), чтобы загрузчик мог определить наличие настроек в EEPROM-памяти
    {0, 0},         // AD9952 -5В
    {2048, 2048},   // AD9952 0В
    {4095, 4095},   // AD9952 +5В
    {0, 0},
    {0, 0},
    {0, 0}
};

CalibrationSettings setCal = defSet;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CalibrationSettings::Load()
{
    *this = defSet;                     // Сначала заполняем значениями по умолчанию - вдруг сохранённых настроек нету
    EEPROM::LoadSettings(this);


}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CalibrationSettings::Save()
{
    EEPROM::SaveSettings(this);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CalibrationSettings &CalibrationSettings::operator =(const CalibrationSettings &rhs)
{
    std::memcpy(this, rhs.unused, sizeof(*this));

    return *this;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CalibrationSettings::CreateMessage(Message *message, Chan ch, KoeffCal::E koeff)
{
    static const struct StructCal
    {
        int16 *pointer;
        StructCal(int16 *p) : pointer(p) {};
    }
    values[KoeffCal::Number] =
    {
        &CAL_AD9952_OFFSET_NEG(Chan::A),
        &CAL_AD9952_OFFSET_ZERO(Chan::A),
        &CAL_AD9952_OFFSET_POS(Chan::A),
        &CAL_AD9952_AMPLITUDE(Chan::A),
        &CAL_DDS_MAX(Chan::A),
        &CAL_DDS_MIN(Chan::A)
    };

    message->AllocateMemory(5);
    message->PutByte((uint8)Command::SetKoeffCalibration);
    message->PutByte((uint8)ch);
    message->PutByte((uint8)koeff);
    message->PutHalfWord((uint16)values[koeff].pointer[ch]);

    uint8 *data = message->Data();

    BitSet16 bs(data + 3);

    LOG_WRITE("Пишу %d %d %d", *(data + 1), *(data + 2), (int16)bs.halfWord);
}
