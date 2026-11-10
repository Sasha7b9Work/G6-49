// (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "common/Messages.h"
#include "Utils/List.h"


struct Task;


namespace PInterface
{
    // Для приёма сообщений от устройства
    void Update();
    
    void Transmit(SimpleMessage *);

    bool Receive(SimpleMessage *);

    // Добавляеет задание. Если не получилось, возвращает false
    void AddTask(Task *);

    void AddMessageForTransmit(SimpleMessage *);
};


// Структура содержит данные для элементарного задания, передаваемого в Interface
struct Task
{
    // Этот конструктор может использоваться только в Interface для сравнения
    Task(SimpleMessage *);

    Task(SimpleMessage *, bool (*process)(SimpleMessage *), bool (*equal)(Task *, Task *));

    // Деструктор. В нём нужно удалить сообщение
    ~Task();

    // Функция сравнения
    bool Equals(Task *, Task *);

    // Соообщение для пересылки
    SimpleMessage *message;

    // Возвращает указатель на готовое к использованию (со сброщенным указателем) сообщение
    SimpleMessage *GetMessage();

    // Послать сообщение задания
    void TransmitMessage();

    // Возвращает true, если прошло слишком мало времени после предыдущей засылки сообщения
    bool PassedLittleTimeAfterSend();

    // Функция обработки ответа
    bool(*funcProcess)(SimpleMessage *);

private:

    // Время последней передачи сообщения
    uint timeLast;

    // Функция сравнения
    bool (*funcEqual)(Task *, Task *);
};
