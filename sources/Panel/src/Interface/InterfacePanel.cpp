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
/// Обработать имеющиеся задания
static void ProcessTasks();



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
        if (Handlers::Processing(&message))
        {
            time = 0;
            Update();
        }
    }

    ProcessTasks();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::AddTask(Task *task)
{
    if (!tasks.Member(task))                    // Если задания ещё нет в очереди
    {
        Send(task->message);                    // То посылаем сообщение
        task->timeLast = TIME_MS;               // запоминаем время посылки
        tasks.Append(task);                     // и добавляем в очередь сообщений для повторной отправки
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ProcessTasks()
{
    ListElement<Task> *element = tasks.First();

    while (element)
    {
        Task *task = element->Get();

        element = tasks.Next();
    }
}
