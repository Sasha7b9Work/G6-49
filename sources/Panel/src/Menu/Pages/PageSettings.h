#pragma once


class PageSignals
{
public:
    static void Init();
    static void OnPress_Form(bool = true);

    static ChoiceParameter *cpParameters;
    static Page *pointer;
};
