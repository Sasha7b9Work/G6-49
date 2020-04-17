#include "defines.h"
#include "Messages_pl.h"
#include "Hardware/HAL/HAL.h"


Message::RequestUpgrade::RequestUpgrade() : SimpleMessage(1, Command::RequestUpgrade)
{

}


Message::StartMainApplication::StartMainApplication() : SimpleMessage(1, Command::StartApplication)
{

}


Message::PortionUpgradeDevice::PortionUpgradeDevice(int portion) : SimpleMessage(5, Command::PortionUpgradeDevice)
{
    PutINT(portion);
}


Message::AnswerUpgradePanel::AnswerUpgradePanel(int size) : SimpleMessage(5, Command::AnswerUpgradePanel)
{
    PutINT(size);
}


Message::RequestPortionUpgradePanel::RequestPortionUpgradePanel(int16 num) : SimpleMessage(3, Command::RequestPortionUpgradePanel)
{
    PutINT16(num);
}


Message::AnswerPortionUpgradePanel::AnswerPortionUpgradePanel(int16 num, uint8 data[SIZE_CHUNK]) :
    SimpleMessage(1 + sizeof(int16) + sizeof(uint) + SIZE_CHUNK, Command::AnswerPortionUpgradePanel)
{
    PutINT16(num);

    uint crc = HAL_CRC32::CalculateALIGN32(data, SIZE_CHUNK);

    PutUINT(crc);

    PutData(data, SIZE_CHUNK);
}
