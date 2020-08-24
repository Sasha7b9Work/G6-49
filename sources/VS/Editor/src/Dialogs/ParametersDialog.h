#pragma once


class TextControl;


class ParametersDialog : public wxDialog
{
public:
    ParametersDialog(uint16 index, uint16 value);
private:
    
    TextControl *tcPosition;      // Здесь будем вводить индекс точки

    TextControl *tcValue;         // Здесь будем вводить числовое значение точки

    void OnButtonApply(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);
};
