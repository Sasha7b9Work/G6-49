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
    PutWord(portion);
}

                                                                                                      // com + sizeof(num) + sizeof(portion) + sizeof(uint8) * size
Message::PortionUpgradePanel::PortionUpgradePanel(int num, int portion, uint8 *data, int size) : SimpleMessage(static_cast<uint>(1 + 4 + 4 + size), Command::PortionUpgradePanel)
{
    PutWord(portion);
    PutWord(num);
    for(int i = 0; i < size; i++)
    {
        PutByte(*data++);
    }
}
