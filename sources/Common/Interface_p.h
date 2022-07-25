// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Messages.h"
#include "Utils/List.h"




// Структура содержит данные для элементарного задания, передаваемого в Interface
struct Task
{
    // Этот конструктор может использоваться только в Interface для сравнения
    Task(SimpleMessage *msg);

    Task(SimpleMessage *msg, bool (*process)(SimpleMessage *), bool (*equal)(Task *, Task *));
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



class PInterface
{
public:
    // Для приёма сообщений от устройства
    static void Update();
    // Добавляеет задание. Если не получилось, возвращает false
    static void AddTask(Task *task);

    static void AddMessageForTransmit(SimpleMessage *message);
private:
    // Обрабатывает answer, если запрос на него есть в очереди заданий. Возвращает true, если это так
    static bool ProcessTask(SimpleMessage *answer);
    // Обрабатывает очередь заданий, засылая сообщения тех из них, которые необходимо заслать
    static void SendTasks();
    // Обработать ответ на задание
    static void RunAnswer(ListElement<Task> *element, SimpleMessage *answer);
};
