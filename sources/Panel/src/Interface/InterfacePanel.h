#pragma once
#include "Message.h"
#include "Utils/List.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Структура содержит данные для элементарного задания, передаваемого в Interface
struct Task
{
friend class Interface;

    Task(Message *msg, bool (*process)(Message *), bool (*equal)(Task *, Task *));
    /// Деструктор. В нём нужно удалить сообщение
    ~Task();
    /// Функция сравнения
    bool Equals(Task *, Task *);
    /// Соообщение для пересылки
    Message *message;
    /// Возвращает указатель на готовое к использованию (со сброщенным указателем) сообщение
    Message *GetMessage();
private:
    /// Этот конструктор может использоваться только в Interface для сравнения
    Task(Message *msg);
    /// Время последней передачи сообщения
    uint timeLast;
    /// Функция сравнения
    bool (*funcEqual)(Task *, Task *);
    /// Функция обработки ответа
    bool (*funcProcess)(Message *);
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Interface
{
public:

    /// Для приёма сообщений от устройства
    static void Update();

    static void Send(Message *message);
    /// Добавляеет задание. Если не получилось, возвращает false
    static void AddTask(Task *task);

private:
    /// Обрабатывает answer, если запрос на него есть в очереди заданий. Возвращает true, если это так
    static bool ProcessTask(Message *answer);
    /// Обрабатывает очередь заданий, засылая сообщения тех из них, которые необходимо заслать
    static void SendTasks();
    /// Возвращает true, если прошло слишком мало времени после предыдущей засылки сообщения
    static bool PassedLittleTimeAfterSend(Task *task);
    /// Обработать ответ на задание
    static void RunAnswer(ListElement<Task> *element, Message *answer);

};
