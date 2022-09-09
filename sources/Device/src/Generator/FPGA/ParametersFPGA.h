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
        }
    }
}
