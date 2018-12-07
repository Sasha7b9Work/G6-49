#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "List.h"
#include "Interface/InterfacePanel.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template void               List<Task>::Append(ListElement<Task> *);
template bool               List<Task>::Member(ListElement<Task> *);
template ListElement<Task> *List<Task>::First();
template                    List<Task>::List();
template void               List<Task>::Remove(ListElement<Task> *);

template                    ListElement<Task>::~ListElement();
template Task              *ListElement<Task>::Get();
template ListElement<Task> *ListElement<Task>::Next();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
List<T>::List() : head(nullptr)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void List<T>::Append(ListElement<T> *element)
{
    element->next = nullptr;
    element->prev = nullptr;
    element->owningList = this;

    ListElement<T> *last = Last();

    if (last == nullptr)
    {
        head = element;
    }
    else
    {
        last->next = element;
        element->prev = last;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void List<T>::Remove(ListElement<T> *removed)
{
    ListElement<T> *element = First();

    while (element)
    {
        if (element == removed)
        {
            /// Здесь надо удалить

            ListElement<T> *next = element->next;
            ListElement<T> *prev = element->prev;

            if (element == First())             // Если удалемый элемент - первый в списке
            {
                if (next)
                {
                    next->prev = nullptr;       // То делаем первым следующий элемент
                }
                head = next;                    // И записываем его в голову
            }
            else if (element == Last())         // Если удаляемый элемент - последний в списке
            {
                prev->next = nullptr;           // То просто запиываем в предыдущий элемент признак конца
            }
            else
            {
                next->prev = prev;
                prev->next = next;
            }

            return;
        }

        element = element->Next();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
ListElement<T> *List<T>::Last()
{
    if (head == nullptr)
    {
        return nullptr;
    }

    ListElement<T> *element = First();

    while (element->next != nullptr)
    {
        element = element->Next();
    }

    return element;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
bool List<T>::Member(ListElement<T> *element)
{
    ListElement<T> *member = First();

    T *el = element->Get();

    while (member != nullptr)
    {
        if (member->Get()->Equals(member->Get(), el))
        {
            return true;
        }

        member = member->Next();
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
ListElement<T> *List<T>::First()
{
    return head;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
T *ListElement<T>::Get()
{
    return value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
ListElement<T> *ListElement<T>::Next()
{
    return next;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
ListElement<T>::~ListElement()
{
    delete value;
}
