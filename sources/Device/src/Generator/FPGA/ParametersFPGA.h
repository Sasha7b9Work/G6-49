// 2022/09/09 09:22:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Common/Common.h"
#include "Common/CommonTypes.h"


namespace FPGA
{
    namespace Packet
    {
        namespace Period
        {
            void Set(const Value &);

            // Записывает ранее установленное значение
            void Write();
        }

        // Число импульсов в пакете
        namespace Number
        {
            // Устанавливает число импульсов в пачке
            void Set(const uint n);
        }
    }

    namespace Impulse
    {
        namespace Duration
        {
            void Set(const Chan &, const Value &);
            const Value &Gurrent(const Chan &);

            // Записывает ранее установленное значение
            void Write(const Chan &);
        }

        namespace Period
        {
            void Set(const Chan &, const Value &);
            const Value &Current(const Chan &);

            // Записывает ранее установленные значения
            void Write(const Chan &);
        }
    }

    namespace StartStop
    {
        namespace Mode
        {
            enum E
            {
                Disable,
                Enable
            };

            void Set(E);
            E Current();
        }


        namespace Delay
        {
            // Задержка между импульсами на выходах первого и второго каналов
            void Set(const Value &delay);

            // Записывает ранее установленные значения
            void Write();
        }
    }
}
