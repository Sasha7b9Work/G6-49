#pragma once


namespace SCPI
{
    namespace Error
    {
        namespace Syntax
        {
            /// Неправильный синтаксис
            void Invalid();
            /// Пропущено двоеточие в начале команды
            void MissingColon();
        }

        /// Неизвестная команда
        void UnknownCommand();
    }
}
