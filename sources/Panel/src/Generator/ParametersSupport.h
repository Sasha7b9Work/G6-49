#pragma once
#include "Settings/SettingsTypes.h"
#include "Generator/Parameters.h"


struct FloatValue;
class Parameter;


/// Вспомогательный класс для отрисовки значения параметра
class ParameterPainterSupporting
{
public:
    ParameterPainterSupporting();
    /// Назначение обрарбатываемого параметра
    void SetParameter(Parameter *param);
    /// Возращает количество отрисовываемых символов
    uint NumSymbols() const;
    /// Возвращает позицию X символа с позицией pos
    int X(uint pos) const;
    /// Возвращает символ в позиции pos
    char Symbol(uint pos) const;
    /// Координата Х верхнего левого угла обласити вывода
    int X0() const;
    /// Координата Y верхнего левого угла области вывода
    int Y0() const;
    /// Ширина области вывода
    int Width() const;
    /// Обрабатываемый параметр
    Parameter *parameter;
    /// Возвращает позицию активного символа
    //uint GetPositionActive() const { return static_cast<uint>(positionActive); };
    /// Ширина поля разряда
    int WidthDigit() const { return 11; };
    /// Высотал поля разряда
    int HeightDigit() const { return 17; }

public:
    

    /// Позиция первого символа
    int x0;

    /// Возвращает позицию первого символа единиц измерения
    uint PositionFirstUnit() const;
    /// Инициалзировать позицию активного символа
    void InitPositionActive();

};


/// Используется для настройки параметра с клавиатуры
class ParameterTuner
{
public:
    ParameterTuner() {}
    /// Установить параметр, подлежащий настройке
    void SetParameter(Parameter *param);
    /// Отрисовать параметр в режиме настройки
    void Draw();

    Parameter *GetParameter();

private:
    ParameterPainterSupporting support;
    /// Отрисовать имя параметра
    void DrawNameParameter();
    /// Отрисовать параметр
    void DrawParameter();
    /// Отрисовать подсвечиватель в соотвествующей позиции
    void DrawHighlighter();
};



