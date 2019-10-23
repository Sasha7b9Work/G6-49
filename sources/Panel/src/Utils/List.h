#pragma once



template <class T> class List;


template <class T>
class ListElement
{
friend class List<T>;

public:

    ListElement(T *v) : value(v), next(nullptr), prev(nullptr) {};

    ~ListElement();

    T *Get();
    /// Возвращает указатель на следующий элемент
    ListElement *Next();
private:

    T* value;
    /// Указатель на список - владелец
    //List<T> *owningList;
    /// Указатель на следующий элемент. 0, если это это последний элемент в списке
    ListElement *next;
    /// Указатель на предыдущий элемент. 0, если это первый элемент в списке
    ListElement *prev;
};



template <class T>
class List
{
public:
    List();
    /// Добавляем новый элемент в конец списка
    void Append(T *element);
    /// Удаляет элемент из списка
    void Remove(T *element);
    /// Возвращает true, если список содержит element
    bool IsMember(T *element);
    /// Возвращает указатель
    ListElement<T> *First();
    /// Возвращает размер
    uint Size() const;

    void Clear();

private:
    /// Возвращает указатель на последний элемент списка
    ListElement<T> *Last();
    /// Указатель на первый элемент списка. 0, если список пуст
    ListElement<T> *head;
};
