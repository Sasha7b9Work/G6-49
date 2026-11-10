// (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "common/Interface_p.h"
#include "common/Command.h"
#include "structs.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Debug.h"
#include "common/Handlers_p.h"
#include <cstdlib>
#include <cstring>


static List<Task> tasks;


namespace PInterface
{
    // Обрабатывает answer, если запрос на него есть в очереди заданий. Возвращает true, если это так
    static bool ProcessTask(SimpleMessage *answer);

    static void ReceiveMessage();

    // Засылает одно сообщение
    static void SendMessage();

    // Обработать ответ на задание
    static void RunAnswer(ListElement<Task> *element, SimpleMessage *answer);
}


void PInterface::AddMessageForTransmit(SimpleMessage *message)
{
    Transmit(message);
}


void PInterface::Update()
{
    if (CPU::IsReady())     // У device нет данных для передачи
    {
        SendMessage();
    }
    else                    // У device есть данные для передачи
    {
        ReceiveMessage();
    }
}


void PInterface::ReceiveMessage()
{
    uint8 flag = 0;

    HAL_SPI4::TransmitReceive(0, &flag);

    bool error_flag = false;

    if (flag == 0xFF)
    {
        static const int SIZE_BUFFER = 1024;
        uint8 out[SIZE_BUFFER];
        uint8 in[SIZE_BUFFER] = { 0xFF };

        std::memset(out, 0, SIZE_BUFFER);

        HAL_SPI4::TransmitReceive(out, in + 1, 7, 20);         // Получаем оставшиеся 3 байта заголовка и 4 байта размера

        uint *header = (uint *)in;

        if (*header != 0xFFFFFFFF)                              // Проверяем корректность заголовка
        {
            error_flag = true;
        }
        else
        {
            int *size = (int *)in[4];

            if (*size >= SIZE_BUFFER || *size == 0)                    // Проверяем, что размер в допустимых пределах
            {
                error_flag = true;
            }
            else
            {
                HAL_SPI4::TransmitReceive(out, in, (uint16)*size, 200);     // Принимаем собственно сообщение и 

                uint *crc32_msg = (uint *)in[*size - 4];

                uint crc32_real = HAL_CRC32::Calculate(in, (uint)(*size - 4));

                if (*crc32_msg != crc32_real)
                {
                    error_flag = true;
                }
                else
                {
                    SimpleMessage message;
                    message.CreateAllocate(in, *size - 4);

                    if (!ProcessTask(&message))
                    {
                        PHandlers::Processing(&message);
                    }
                }
            }
        }
    }

    if (error_flag)
    {
        HAL_SPI4::Transmit(0xFF);   // Произошла ошибка во время приёма информации.
                                    // По принятому значению 0xFF device поймёт это и повторит передачу
    }
}


void PInterface::Transmit(SimpleMessage *)
{

}


void PInterface::AddTask(Task *task)
{
    if (!tasks.IsMember(task))                   // Если задания ещё нет в очереди
    {
        task->TransmitMessage();                // То посылаем его сообщение
        tasks.Append(task);                     // и добавляем в очередь сообщений для повторной отправки
    }
}


void Task::TransmitMessage()
{
    message->Transmit();    // Посылаем сообщение
    timeLast = _TIME_MS;     // запоминаем время посылки
}


void PInterface::SendMessage()
{
    ListElement<Task> *element = tasks.First();

    if (element)
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
    return (_TIME_MS - timeLast) < 1000;
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



