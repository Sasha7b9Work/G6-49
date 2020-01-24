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
    uint GetPositionActive() const { return static_cast<uint>(positionActive); };

    /// Сделать активным следующий символ
    void SetActiveNext();
    /// Сделать активным предыдущий символ
    void SetActivePrev();
    /// Увеличить значение в текущем разряде
    void IncreaseInCurrentPosition();
    /// Уменьшить значение в текущем разрядe
    void DecreaseInCurrentPosition();
    /// Ширина поля разряда
    int WidthDigit() const { return 11; };
    /// Высотал поля разряда
    int HeightDigit() const { return 17; }

public:
    static const int NUM_SYMBOLS = 50;
    /// Здесь хранятся отрисовываемые символы
    char buffer[NUM_SYMBOLS];
    /// Здесь хранятся позиции символов из FloatValue, соответствующие цифровым символам в buffer. Для нецифровых символов == 255;
    int8 indexes[NUM_SYMBOLS];
    /// Позиция первого символа
    int x0;
    /// Позиция активного символа
    int positionActive;
    /// Возвращает позицию первого символа единиц измерения
    uint PositionFirstUnit() const;
    /// Инициалзировать позицию активного символа
    void InitPositionActive();
    /// Возвращает true, если символ в данной позиции может быть подсвечен
    bool PositionMayBeActived(int pos);
    /// Делает попытку изменить знак числа и, если удалось возвращает true
    bool ChangedSign();
};


/// Используется для настройки параметра с клавиатуры
class ParameterTuner
{
public:
    ParameterTuner() {}
    /// Установить параметр, подлежащий настройке
    void SetParameter(Parameter *param);
    /// Обработать событие клавиатуры. Возвращает true, если обработано
    bool ProcessControl(const Key &key);
    /// Отрисовать параметр в режиме настройки
    void Draw();

private:
    ParameterPainterSupporting support;
    /// Отрисовать имя параметра
    void DrawNameParameter();
    /// Отрисовать параметр
    void DrawParameter();
    /// Отрисовать подсвечиватель в соотвествующей позиции
    void DrawHighlighter();
};


/// Структура используется для отрисовки значения параметра
class ParameterPainter
{
public:
    static void SetPatameter(Parameter *param)
    {
        parameter = param;
    }

    /// Возвращает строку значения параметра со знаком для редактитрования. В массиве indexes сохраняются позиции символов относительно точки (в предположении, что точка стоит после символа в нулевой позиции)
    static pString DigitsWithSign(int8 *indexes);
    /// Возвращает строку с единицами измерения
    static pString Units(Language::E lang = Language::RU);

private:
    static Parameter *parameter;
    /// Возвращает строку значения параметра ParameterValue
    static pString DigitsWithSignValue(int8 *indexes);
    /// Возвращает строку значения смещения
    static pString DigitsWithSignOffset(int8 *indexes);
    /// Возвращает единицы измерения ParameterValue
    static pString UnitsValue(Language::E lang = Language::RU);
};


struct Order
{
    enum E
    {
        Mega,
        Kilo,
        One,
        Milli,
        Micro,
        Nano,
        Count
    } value;

    pString Suffix(Language::E lang) const;
};


struct MathFloatValue
{
    /// Возвращает числовое представление FloatValue с numDigits значащими цифрами и запятой. В order сохраняется порядок числа относительно запятой
    static pString GetStringValue(const FloatValue &value, bool sign, int numDigits, Order::E *order, int posFirst = 255);
    /// Возвращает позицию первого ненулевого символа
    /// "1" - десятки (1e1), "0" - единицы (1e0), "-1" - десятые (1e-1), "-2" - сотые (1e-2)
    static int GetPositionFirstDigit(const FloatValue &value);
    /// Возвращает цифру в позиции position
    static int GetDigit(const FloatValue &value, int position);
    /// Возвращает символ в позиции position
    static char GetChar(const FloatValue &value, int postition);
    /// Возвращает позицию запятой относительно позиции первого значащего символа
    static int PositionComma(int posFirstDigit, Order::E *order);
};


struct LogicFloatValue
{
    static void SetParameter(Parameter *param);
    /// Делает попытку изменить знак и возвращает true, если это возможно
    static bool ChangedSign();
    /// Изменить значение числа в позиции pos, где pos - номер десятичного разряда, начиная от 10e0
    static void ChangeDigit(int pos, int delta);

private:
    
    static FloatValue *value;

    static ParameterValue *parameter;
};
