#pragma once
#include "Dialog.h"


class ExponentDialog : public Dialog
{
public:
    ExponentDialog();

protected:

    virtual void SendAdditionForm();

private:
    void DrawLine(int x1, int y1, int x2, int y2);
};
