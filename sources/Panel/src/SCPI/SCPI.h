#pragma once
#include "Message.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace SCPI
{
    struct StructCommand
    {
        char  *symbols;
        void (*func)();
    };

    void Init();

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Handler
    {
    public:
        static bool Processing(SimpleMessage *msg);
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct Buffer
    {
        /// Очистка буфера - подготовка к работе
        static void Clear();
        /// Добавляет данные из сообщения в буфер
        static void AddData(SimpleMessage *msg);
        /// Выполняет команду, если она есть в буфере, и удаляет использованные и неверные данные
        static void RunAndCompress();
    private:
        static void AddByte(uint8 byte);
        static const int SIZE_BUFFER = 1024;
        static uint8 data[SIZE_BUFFER];
        static int   used;
    };

    extern Buffer buffer;

    /// Выполнение команды. Если команда выполнена, выполняется адрес последнего символа команды (это символ завершения строки 0x0d)
    uint8 *ProcessingCommand(const StructCommand *commands, uint8 *data);

    namespace Process
    {
        void IDN();
        void RST();
        void Channel();
        void Frequency();
        void Form();
        void Amplitude();
    }
};
