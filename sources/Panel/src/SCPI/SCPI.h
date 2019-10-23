#pragma once
#include "Message.h"



namespace SCPI
{
    struct Result
    {
        enum E
        {
            Done,               ///< Успешно выполнено
            IncompleteCommand,  ///< Команда не принята полностью
            InvalidParameter,   ///< Неправильный параметр
            Count
        };
    };

    void Init();

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Handler
    {
    public:
        static bool Processing(SimpleMessage *msg);
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct Buffer
    {
        /// Очистка буфера - подготовка к работе
        static void Clear();
        /// Добавляет данные из сообщения в буфер
        static void AddData(SimpleMessage *msg);
        /// Возвращает байт в позиции i
        static uint8 GetByte(uint i);

        static uint Size() { return static_cast<uint>(used); };
        /// Удалить неправльные символы из начала буфера
        static void RemoveBadSymbols();

        static void MarkFirstSymbolAsBad() { data[0] = '\0'; };

    private:
        static void AddByte(uint8 byte);
        /// Сдвинуть содержимое буфера на один байт влево. Самый первый байт при этом теряется
        static bool ShiftToLeft();

        static const int SIZE_BUFFER = 1024;
        static uint8 data[SIZE_BUFFER]; ///< Здесь принятые данные хранятся
        static int   used;              ///< Количество готовых к обработке байт - первый обработываемый байт имеет индекс 0, последний = used - 1
        static int   start;             ///< Индекс первого байта обрабатываемого шага. Если первый байт команды всегда имеет индекс 0, то 
    };
};
