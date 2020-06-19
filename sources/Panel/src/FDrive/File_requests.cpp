#include "defines.h"
#include "common/Command.h"
#include "common/Interface_p.h"
#include "common/Messages.h"
#include "Display/Text.h"
#include "FDrive/File.h"
#include "FDrive/Items.h"


static bool EqualsRequestString(Task *task1, Task *task2)
{
    SimpleMessage *msg1 = task1->GetMessage();
    SimpleMessage *msg2 = task2->GetMessage();

    uint8 com = Command::FDrive_RequestFileString;

    return  (com == msg1->TakeUINT8()) &&
            (com == msg2->TakeUINT8()) &&
            (msg1->TakeUINT8() == msg2->TakeUINT8());
}

void FileRequests::SendRequestForString(const String *name, int numString)
{
    Message::FDrive::FileString message(static_cast<uint>(numString), name->c_str());

    Task *task = new Task(&message, Items::Handler::Processing, EqualsRequestString);

    PInterface::AddTask(task);
}
