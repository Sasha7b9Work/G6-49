#include "defines.h"
#include "Messages_pl.h"


Message::RequestUpgrade::RequestUpgrade() : SimpleMessage(1, Command::RequestUpgrade)
{

}


Message::StartMainApplication::StartMainApplication() : SimpleMessage(1, Command::StartApplication)
{

}


Message::PortionUpdateDevice::PortionUpdateDevice(int portion) : SimpleMessage(5, Command::PortionUpgradeDevice)
{
    PutWord(portion);
}
