#include "defines.h"
#include "Messages_pl.h"


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

                                                                                                      // com + sizeof(num) + sizeof(portion) + sizeof(uint8) * size
Message::PortionUpgradePanel::PortionUpgradePanel(int num, int portion, uint8 *data, int size) : SimpleMessage(1 + 4 + 4 + size, Command::PortionUpgradePanel)
{
    PutINT(portion);
    PutINT(num);
    PutData(data, size);
}


Message::EraseSectors::EraseSectors(int num) : SimpleMessage(5, Command::EraseSectors)
{
    PutINT(num);
}
