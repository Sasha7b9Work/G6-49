#include "defines.h"
#include "common/Transceiver.h"
#include "Interface_pl.h"
#include "common/Command.h"
#include "structs.h"
#include "FDrive/FDrive_p.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Queue.h"
#include "Handlers_pl.h"
#include "Interface_pl.h"
#include "Utils/Debug.h"
#include <cstdlib>


static List<Task> tasks;


void PInterface::AddMessageForTransmit(SimpleMessage *message)
{
    Transceiver::Transmit(message);
}


void PInterface::Update()
{
    static uint time = 0;

    if (TIME_MS - time < 100)
    {
        return;
    }

    time = TIME_MS;

    DEBUG_POINT_0;

    Message::RequestData message;

    DEBUG_POINT_0;

    Transceiver::Transmit(&message);

    DEBUG_POINT_0;
    
    if (Transceiver::Receive(&message))
    {
        DEBUG_POINT_0;

        if (ProcessTask(&message) ||            // ќбрабатываем сообщение, если запрос на него есть в очереди заданий
            PHandlers::Processing(&message))    // или просто обрабатываем в обратном случае
        {
            DEBUG_POINT_0;
            time = 0;
            Update();
            DEBUG_POINT_0;
        }
    }

    DEBUG_POINT_0;

    SendTasks();
}


void PInterface::AddTask(Task *task)
{
    if(!tasks.IsMember(task))                   // ≈сли задани€ ещЄ нет в очереди
    {
        task->TransmitMessage();                // “о посылаем его сообщение
        tasks.Append(task);                     // и добавл€ем в очередь сообщений дл€ повторной отправки
    }
}


void Task::TransmitMessage()
{
    message->Transmit();    // ѕосылаем сообщение
    timeLast = TIME_MS;     // запоминаем врем€ посылки
}


void PInterface::SendTasks()
{
    ListElement<Task> *element = tasks.First();

    while (element)
    {
        Task *task = element->Get();

        if (!task->PassedLittleTimeAfterSend())
        {
            task->TransmitMessage();
        }

        element = element->Next();
    }
}


bool Task::PassedLittleTimeAfterSend()
{
    return (TIME_MS - timeLast) < 1000;
}


bool PInterface::ProcessTask(SimpleMessage *answer)
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


void PInterface::RunAnswer(ListElement<Task> *element, SimpleMessage *answer)
{
    element->Get()->funcProcess(answer);
    tasks.Remove(element->Get());
    delete element;
}


bool Task::Equals(Task *task1, Task *task2)
{
    return funcEqual(task1, task2);
}


Task::Task(SimpleMessage *msg) : funcProcess(nullptr), timeLast(0), funcEqual(nullptr)
{
    message = msg->Clone();
}


Task::Task(SimpleMessage *msg, bool(*process)(SimpleMessage *), bool(*equal)(Task *, Task *)) :funcProcess(process), timeLast(0), funcEqual(equal)
{
    message = msg->Clone();
}


Task::~Task()
{
    delete message;
}


SimpleMessage *Task::GetMessage()
{
    message->ResetPointer();
    return message;
}


void SimpleMessage::Transmit()
{
    PInterface::AddMessageForTransmit(this);
}
