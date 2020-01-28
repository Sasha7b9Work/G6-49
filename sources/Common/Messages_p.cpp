#include "defines.h"
#include "Messages_p.h"


Message::LoadFormDDS::LoadFormDDS(uint8 ch, uint16 data[PGenerator::DDS_NUM_POINTS]) : SimpleMessage(2 + PGenerator::DDS_NUM_POINTS * 2, Command::LoadFormDDS)
{
    PutByte(ch);
    PutData(reinterpret_cast<uint8*>(data), sizeof(data[0]) * PGenerator::DDS_NUM_POINTS);
}
