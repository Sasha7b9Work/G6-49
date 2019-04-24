#pragma once


namespace SCPI
{
    namespace Runner
    {
        /// Выполняет команду из SCPI::Parser::words
        bool Execute();

        struct Result
        {
            enum E
            {
                IsOk,               ///< Успешное выполнение команды
                InvalidSyntax,      ///< Неправильный синтаксис команды
                UnknownCommand,     ///< Неизвестная комманда
                Count
            };
        };

        struct StructCommand
        {
            char      *symbols;
            Result::E(*func)();
        };

        void ProcessError(Result::E error);

        void HandlerInvalidSyntax();
        void HandlerUnknownCommand();

        namespace Process
        {
            Result::E IDN();
            Result::E RST();
            Result::E Channel();
            Result::E Frequency();
            Result::E Form();
            Result::E Amplitude();
        }
    }
}
