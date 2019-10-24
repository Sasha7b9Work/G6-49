#pragma once

/*
     ласс представлени€ формы сигнала
*/


class Form
{
public:
    Form();
    void SetPoint(int mouseX, int mouseY);
    /// ¬озвращает true, если курсор мыши находитс€ над поставленной точкой
    bool ExistPoint(int mouseX, int mouseY);
    void Draw();
};


extern Form *TheForm;
