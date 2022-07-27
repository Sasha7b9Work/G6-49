// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Editor/Editor.h"
#include "Editor/Settings.h"


ModeButtonLeft::E modeButtonLeft = ModeButtonLeft::EditLines;


ModeButtonLeft::E ModeButtonLeft::Get()
{
    return modeButtonLeft;
}


void ModeButtonLeft::Set(ModeButtonLeft::E v)
{
    modeButtonLeft = v;

    TheFrame->SetModeMBL();
}
