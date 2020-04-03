#include "defines.h"
#include "Messages_pl.h"


Message::RequestUpdate::RequestUpdate() : SimpleMessage(1, Command::RequestUpdate)
{

}


Message::StartMainApplication::StartMainApplication() : SimpleMessage(1, Command::StartApplication)
{

}


Message::PortionUpdateDevice::PortionUpdateDevice(int portion) : SimpleMessage(5, Command::PortionUpdateDevice)
{
    PutWord(portion);
}
