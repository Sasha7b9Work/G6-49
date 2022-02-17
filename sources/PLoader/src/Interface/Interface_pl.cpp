#include "defines.h"
#include "common/Interface_p.h"
#include "common/Transceiver.h"
#include "common/Command.h"
#include "structs.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Queue.h"
#include "Handlers_pl.h"
#include <cstdlib>


static List<Task> tasks;


void PInterface::AddMessageForTransmit(SimpleMessage *message)
{
    Transceiver::Transmit(message);
}


void PInterface::Update()
{
    Message::RequestData message;

    Transceiver::Transmit(&message);
    
    if (Transceiver::Receive(&message))
    {
        if (ProcessTask(&message) ||            // ������������ ���������, ���� ������ �� ���� ���� � ������� �������
            PHandlers::Processing(&message))    // ��� ������ ������������ � �������� ������
        {
        }
    }

    SendTasks();
}


void PInterface::AddTask(Task *task)
{
    if(!tasks.IsMember(task))                   // ���� ������� ��� ��� � �������
    {
        task->TransmitMessage();                // �� �������� ��� ���������
        tasks.Append(task);                     // � ��������� � ������� ��������� ��� ��������� ��������
    }
}


void Task::TransmitMessage()
{
    message->Transmit();    // �������� ���������
    timeLast = TIME_MS;     // ���������� ����� �������
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
