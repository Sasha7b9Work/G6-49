#pragma once
#include "defines.h"

/*
    Класс представления формы сигнала
*/


class Form
{
public:
    Form();
    /// Очистить сигнал
    void Clear();
    /// Добавить точку с координатами мыши
    void SetPoint(int mouseX, int mouseY);
    /// Добавить точку с абсолютными значениями
    void SetPoint(uint16 pos, uint16 data);
    /// Возвращает true, если курсор мыши находится над поставленной точкой. Pressed - нажата ли кнопка мыши
    bool ExistPoint(int mouseX, int mouseY, bool pressed);
    /// Удалить точку в позиции мыши
    void RemovePoint();
    /// Переместить точку в новую позицию
    void MovePoint(int mouseX, int mouseY);

    void Draw();
};


extern Form *TheForm;
