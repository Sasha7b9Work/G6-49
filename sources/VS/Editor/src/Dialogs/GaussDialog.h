#pragma once
#include "Dialogs/Dialog.h"


class TextControlFloat;


class GaussDialog : public Dialog
{
public:
    GaussDialog();

protected:

    virtual void SendAdditionForm() override;

private:

    TextControlFloat *tcA = nullptr;
    TextControlFloat *tcB = nullptr;
    TextControlFloat *tcC = nullptr;
};
