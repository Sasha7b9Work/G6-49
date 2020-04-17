#include "defines.h"
#include "structs.h"
#include "common/Command.h"
#include "FDrive/FDrive_p.h"
#include "File.h"
#include "Hardware/Timer.h"
#include <cstdlib>
#include <cstring>


FDrive::View   FDrive::view;
SimpleMessage *FDrive::Handler::msg = nullptr;


FDrive::Mount FDrive::mounted = Disconnect;
char          FDrive::directory[255];
bool          FDrive::inStateWaitCompleteLoad = false;


void FDrive::Init()
{
    std::strcpy(directory, "\\");
}

static bool E()
{
    return false;
}


bool FDrive::Handler::Processing(SimpleMessage *message)
{
    message->ResetPointer();

    uint8 com = message->TakeUINT8();

    pFuncBV func = E;

    if(com == Command::FDrive_Mount)
    {
        func = Handler::IsMount;
    }

    return func();
}


bool FDrive::Handler::IsMount()
{
    mounted = static_cast<Mount>(msg->TakeUINT8());
    Init();
    return true;
}


char *FDrive::CurrentDirectory()
{
    return directory;
}
