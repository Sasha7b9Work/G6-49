#pragma once
#include "defines.h"

/*
     ласс представлени€ формы сигнала
*/


class Form
{
public:
    Form();
    /// ƒобавить точку с координатами мыши
    void SetPoint(int mouseX, int mouseY);
    /// ƒобавить точку с абсолютными значени€ми
    void SetPoint(uint16 pos, uint16 data);
    /// ¬озвращает true, если курсор мыши находитс€ над поставленной точкой
    bool ExistPoint(int mouseX, int mouseY);
    /// ”далить точку в позиции мыши
    void RemovePoint();
    /// ѕереместить точку в новую позицию
    void MovePoint(int mouseX, int mouseY);

    void Draw();
};


extern Form *TheForm;
