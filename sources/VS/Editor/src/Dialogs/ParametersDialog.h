// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class TextControlInt;


class ParametersDialog : public wxDialog
{
public:
    ParametersDialog(uint16 index, uint16 value);
private:
    
    TextControlInt *tcPosition;      // Здесь будем вводить индекс точки

    TextControlInt *tcValue;         // Здесь будем вводить числовое значение точки

    void OnButtonApply(wxCommandEvent &);

    void OnButtonClose(wxCommandEvent &);
};
