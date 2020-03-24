#include "defines.h"
#include "common/Command.h"
#include "common/Messages.h"
#include "File.h"
#include "Items.h"
#include "Interface/Interface_p.h"
#include "Utils/String.h"


static bool EqualsRequestString(Task *task1, Task *task2)
{
    SimpleMessage *msg1 = task1->GetMessage();
    SimpleMessage *msg2 = task2->GetMessage();

    uint8 com = Command::FDrive_RequestFileString;

    return  (com == msg1->TakeByte()) &&
            (com == msg2->TakeByte()) &&
            (msg1->TakeByte() == msg2->TakeByte());
}

void FileRequests::SendRequestForString(const String *name, int numString)
{
    Message::FDrive::FileString message(static_cast<uint>(numString), name->c_str());

    Task *task = new Task(&message, Items::Handler::Processing, EqualsRequestString);

    PInterface::AddTask(task);
}
