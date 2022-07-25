// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Messages.h"


namespace Message
{
    class LoadFormDDS : public SimpleMessage
    {
    public:
        LoadFormDDS(uint8 ch, uint16 data[PGenerator::DDS_NUM_POINTS]);
    };
}
