#pragma once


namespace SCPI
{
    namespace Runner
    {
        /// Выполняет команду из SCPI::Parser::words
        bool Execute();

        namespace Process
        {
            void IDN();
            void RST();
            void Channel();
            void Frequency();
            void Form();
            void Amplitude();
        }
    }
}
