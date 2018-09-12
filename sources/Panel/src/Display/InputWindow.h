#pragma once
#include "Hardware/Controls.h"
#include "Command.h"
#include "Menu/MenuItems.h"
#include <string.h>


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *NameUnit(char buffer[10], Order order, Parameter parameter);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Стуктура для обработки нажатия кнопок в окне ввода числового значения
class StructValue
{
public:

    StructValue(Form *f = 0, Parameter *p = 0) : form(f), param(p)  { };

    void Set(Form *form, Parameter *param);

    char *StringValue();

    float Value();

    void DrawInputField(int x, int y);

    void KeyLeft();

    void KeyRight();

    void RegLeft();

    void RegRight();

    void PressKey(Control key);

    void SaveValue();

    Form      *form;
    Parameter *param;

private:
    /// Увеличивает значение в заданной позиции. Возвращает false, если итоговое значение не изменилось.
    bool IncreaseDigit(int num);
    /// Уменьшает значение в заданной позиции. Возвращает false, если итоговое значение не изменилось
    bool DecreaseDigit(int num);
    /// Возвращает true, если все цифры слева от num ранвы нулю. И num тоже
    bool All0LeftWithThis(int num);
    /// Возвращает true, елси все цифры слева и эта являются девятками
    bool All9LeftWithThis(int num);
    /// Сдвигает все разряды вправо
    void ShiftToRight();
    /// Сдвиг всех разрядов влево
    void ShiftToLeft();
    /// Возвращает true, если есть только одна крайняя справа цифра
    bool OnlyOneRigthDigit();
    /// Возвращает число до запятой
    int ValueBeforeComma();
    /// Возвращает число после запятой
    float ValueAfterComma();
    /// Переключает порядок на следующий по возрастанию
    void IncreaseOrder();
    /// Заполняет iws из inputBuffer
    void FillFromInputBuffer();
    /// Заслать текущее значение в генератор
    void SendToGenerator();
};



/// Окно редактирования численного значения параметра
class InputWindow
{
public:
    static void Init();

    static void Draw();

    static void KeyLeft();

    static void KeyRight();

    static void KeyEnter();

    static void ProcessContorl(Control control);

private:
    static void DrawAmplitude(int x, int y);

    static void DrawFrequency(int x, int y);

    static void DrawOffset(int x, int y);

    static void DrawPhase(int x, int y);

    static void DrawDelay(int x, int y);

    static void DrawPeriod(int x, int y);

    static void DrawDutyRatio(int x, int y);

    static void DrawDuration(int x, int y);

    static void DrawDigits(int x, int y);

    static void DrawDepthModulation(int x, int y);

    static void DrawPolarity(int x, int y);

    static void DrawDurationRise(int x, int y);

    static void DrawDurationFall(int x, int y);

    static void DrawDurationStady(int x, int y);

    static void DrawDutyFactor(int x, int y);

    static Parameter *param;

    static StructValue m_iws;
};


#ifdef WIN32
#pragma warning(pop)
#endif
