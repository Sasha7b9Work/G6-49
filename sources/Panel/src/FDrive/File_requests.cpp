#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Command.h"
#include "Message.h"
#include "File.h"
#include "Items.h"
#include "Interface/InterfacePanel.h"
#endif






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
    Message::FDrive::FileString message((uint)numString, name->CString());

    Task *task = new Task(&message, Items::Handler::Processing, EqualsRequestString);

    Interface::AddTask(task);
}
