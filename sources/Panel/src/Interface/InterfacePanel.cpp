#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Transceiver.h"
#include "InterfacePanel.h"
#include "Command.h"
#include "structs.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "FDrive/FDrivePanel.h"
#include "FrequencyMeter/FrequencyMeter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Modules/SPI.h"
#include "Utils/Debug.h"
#include "Utils/Queue.h"
#include "HandlersPanel.h"
#include "InterfacePanel.h"
#include <cstdlib>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static List<Task> tasks;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Interface::Send(Message *message)
{
    Transceiver::Transmit(message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Update()
{
    static uint time = 0;

    if (TIME_MS - time < 100)
    {
        return;
    }

    time = TIME_MS;

    Message message(1, Command::RequestData);

    Transceiver::Transmit(&message);
    
    if (Transceiver::Receive(&message))
    {
        if (
            ProcessTask(&message) ||            // ќбрабатываем сообщение, если запрос на него есть в очереди заданий
            Handlers::Processing(&message))     // или просто обрабатываем в обратном случае
        {
            time = 0;
            Update();
        }
    }

    SendTasks();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::AddTask(Task *task)
{
    if (!tasks.Member(task))                    // ≈сли задани€ ещЄ нет в очереди
    {
        Send(task->message);                    // “о посылаем сообщение
        task->timeLast = TIME_MS;               // запоминаем врем€ посылки
        tasks.Append(task);                     // и добавл€ем в очередь сообщений дл€ повторной отправки
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SendTasks()
{
    ListElement<Task> *element = tasks.First();

    while (element)
    {
        Task *task = element->Get();

        if (!PassedLittleTimeAfterSend(task))
        {
            Send(task->message);
            task->timeLast = TIME_MS;
        }

        element = element->Next();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::PassedLittleTimeAfterSend(Task *task)
{
    return (TIME_MS - task->timeLast) < 1000;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::ProcessTask(Message *answer)
{
    ListElement<Task> *element = tasks.First();

    Task taskAnswer(answer);

    while (element)
    {
        Task *task = element->Get();

        if (task->Equals(task, &taskAnswer))
        {
            RunAnswer(element, answer);
            return true;
        }

        element = element->Next();
    }


    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::RunAnswer(ListElement<Task> *element, Message *answer)
{
    element->Get()->funcProcess(answer);
    tasks.Remove(element->Get());
    delete element;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Task::Equals(Task *task1, Task *task2)
{
    return funcEqual(task1, task2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Task::Task(Message *msg) : timeLast(0), funcEqual(nullptr), funcProcess(nullptr)
{
    message = msg->Clone();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Task::Task(Message *msg, bool(*process)(Message *), bool(*equal)(Task *, Task *)) : message(msg), timeLast(0), funcEqual(equal), funcProcess(process)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Task::~Task()
{
    delete message;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message *Task::GetMessage()
{
    message->ResetPointer();
    return message;
}
