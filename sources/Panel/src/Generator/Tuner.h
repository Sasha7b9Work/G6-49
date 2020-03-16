#pragma once


/*
    Используется для настройки параметра
*/


struct FloatValue;
struct Key;
class Parameter;
class ParameterValue;


// Структура используется для отрисовки значения параметра
class ParameterPainter
{
public:

    ParameterPainter(ParameterValue *param) : parameter(param) { };

    static const int SIZE_BUFFER = 50;
    
    // Возвращает строку значения параметра (если необходимо, то со знаком) для редактитрования. В массиве indexes сохраняются позиции символов относительно точки (в предположении, что точка стоит после символа в нулевой позиции)
    pString Digits(int8 *indexes);
    
    // Возвращает строку с единицами измерения
    pString Units(uint lang);

private:
    ParameterValue *parameter;
    FloatValue *value;
    char buffer[SIZE_BUFFER];
    int8 *indexes;

    // Записать в posBuffer буфера символ, соответствующий цифре с позиции indexDigit FloatValue
    void SetChar(int posBuffer, int8 indexDigit);
    void SetChars(int posBuffer, int8 indexDigit, int8 numDigits);

public:
    void DigitsFrequency();
    void DigitsPeriod();
    void DigitsAmplitude();
    void DigitsOffset();
    void DigitsDuration();
    void DigitsDutyRatio();
    void DigitsPhase();
    void DigitsDelay();
    void DigitsDurationRise();
    void DigitsDurationFail();
    void DigitsDurationStady();
    void DigitsDutyFactor();
    void DigitsManipDuration();
    void DigitsManipPeriod();
    void DigitsPacketPeriod();
    void DigitsPacketNumber();
    void DigitsEmpty();
    void DigitsTime();
};


class Tuner
{
public:

    Tuner(Parameter *param);

    // Обработать событие клавиатуры. Возвращает true, если событие обработано
    bool ProcessControl(const Key &key);

private:

    // Уменьшить значение в текущем разрядe
    void DecreaseInCurrentPosition();

    // Увеличить значение в текущем разряде
    void IncreaseInCurrentPosition();

    // Сделать активным предыдущий символ
    void SetActivePrev();

    // Сделать активным следующий символ
    void SetActiveNext();

    // Делает попытку изменить знак числа и, если удалось возвращает true
    bool ChangedSign();

    // Возвращает true, если символ в данной позиции может быть подсвечен
    bool PositionMayBeActived(int pos);

    Parameter *parameter;

    static const int NUM_SYMBOLS = 50;

    // Здесь хранятся позиции символов из FloatValue, соответствующие цифровым символам в buffer. Для нецифровых символов == 255;
    int8 indexes[NUM_SYMBOLS];

    // Здесь хранятся отрисовываемые символы
    char buffer[NUM_SYMBOLS];

    // Позиция активного символа
    int positionActive;

    ParameterPainter painter;
};
