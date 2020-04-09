#include "defines.h"
#include "Messages_pl.h"


Message::RequestUpgrade::RequestUpgrade() : SimpleMessage(1, Command::RequestUpgrade)
{

}


Message::StartMainApplication::StartMainApplication() : SimpleMessage(1, Command::StartApplication)
{

}


Message::PortionUpdate::PortionUpdate(int mode, int portion) : SimpleMessage(6, Command::PortionUpgrade)
{
    PutByte(static_cast<uint8>(mode));
    PutWord(portion);
}
