#pragma once
#include "Message.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    struct StructCommand
    {
        char       *symbols;
        Result::E (*func)();
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
        /// Выполняет команду, если она есть в буфере, и удаляет использованные и неверные данные
        static bool RunAndCompress();
    private:
        static void AddByte(uint8 byte);
        /// Обрабатывает массив стуктур commands. Возвращает true, если одна из структур обработана - команда выполнена, либо ошибочный параметр
        static bool ProcessStructs(const StructCommand *commands);

        static const int SIZE_BUFFER = 1024;
        static uint8 data[SIZE_BUFFER];
        static int   used;
    };

    extern Buffer buffer;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    namespace Process
    {
        Result::E IDN();
        Result::E RST();
        Result::E Channel();
        Result::E Frequency();
        Result::E Form();
        Result::E Amplitude();
    }
};
