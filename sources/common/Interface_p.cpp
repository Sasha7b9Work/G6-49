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
#include "common/List.h"
#include <cstdlib>
#include <cstring>


namespace PInterface
{
    // Обрабатывает answer, если запрос на него есть в очереди заданий. Возвращает true, если это так
    static bool ProcessTask(SimpleMessage *answer);

    static bool ReceiveMessage();

    // Засылает все задания
    static void SendTasks();

    // Обработать ответ на задание
    static void RunAnswer(Task *, SimpleMessage *answer);

    static List<Task> tasks;
}


void PInterface::Update()
{
    if (CPU::IsReady())     // У device нет данных для передачи
    {
        SendTasks();
    }
    else                    // У device есть данные для передачи
    {
        ReceiveMessage();
    }
}


void PInterface::Test()
{
    uint8 in = 0;

    HAL_SPI4::TransmitReceive(0x7e, &in);

    if (in != 0x81)
    {
        in = in;
    }
}


bool PInterface::ReceiveMessage()
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

    HAL_SPI4::Transmit(error_flag ? 0xFF : 0x00);

    return !error_flag;
}


void PInterface::Transmit(SimpleMessage *message)
{
    while (!CPU::IsReady())
    {
        if (ReceiveMessage())
        {
            break;
        }
    }

    static const uint SIZE_BUFFER = 1024;

    uint8 buffer[SIZE_BUFFER] = { 0xFF, 0xFF, 0xFF, 0xFF };

    int *size = (int *)buffer[4];
    *size = message->Size();
    std::memcpy(&buffer[8], message->TakeData(), (uint)*size);
    uint *crc = (uint *)buffer[8 + *size];
    *crc = HAL_CRC32::Calculate(&buffer[8], (uint)*size);

    HAL_SPI4::Transmit(buffer, (uint16)(*size + 4 + 4 + 4), 500);
}


void PInterface::AddTask(Task *task)
{
    if (!tasks.IsMember(task))              // Если задания ещё нет в очереди
    {
        task->TransmitMessage();            // То посылаем его сообщение
        tasks.Append(task);                 // и добавляем в очередь сообщений для повторной отправки
    }
}


void Task::TransmitMessage()
{
    message->Transmit();    // Посылаем сообщение
    timeLast = _TIME_MS;     // запоминаем время посылки
}


void PInterface::SendTasks()
{
    static uint time = 0;

    if (_TIME_MS - time < 100)
    {
        return;
    }

    time = _TIME_MS;

    Task *task = tasks.First();

    while (task)
    {
        if (!task->PassedLittleTimeAfterSend())
        {
            task->TransmitMessage();
        }

        task = tasks.Next();
    }
}


bool Task::PassedLittleTimeAfterSend()
{
    return (_TIME_MS - timeLast) < 1000;
}


bool PInterface::ProcessTask(SimpleMessage *answer)
{
    Task *task= tasks.First();

    Task taskAnswer(answer);

    while (task)
    {
        if (task->Equals(task, &taskAnswer))
        {
            RunAnswer(task, answer);
            return true;
        }

        task = tasks.Next();
    }


    return false;
}


void PInterface::RunAnswer(Task *task, SimpleMessage *answer)
{
    task->funcProcess(answer);
    tasks.Remove(task);
    delete task;
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
    PInterface::Transmit(this);
}
