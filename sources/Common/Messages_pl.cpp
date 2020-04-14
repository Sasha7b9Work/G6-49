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


Message::AnswerUpgradePanel::AnswerUpgradePanel(int size) : SimpleMessage(5, Command::AnswerUpgradePanel)
{
    PutINT(size);
}
