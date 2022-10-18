// 2022/10/18 14:45:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Common/Common.h"


namespace Filtr
{
    struct Type
    {
        enum E
        {
            None,
            Chebyshev,
            Bessel,
            Count
        };
    };

    void Init();

    void Tune(const Chan &, TypeForm::E);

    void SetType(const Chan &, Type::E);
}
