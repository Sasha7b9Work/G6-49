#pragma once
#include "Hardware/Controls.h"
#include "Command.h"
#include "Menu/MenuItems.h"
#include <string.h>


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Стуктура для обработки нажатия кнопок в окне ввода числового значения
class StructValue
{
public:

    StructValue(ParameterValue *p = 0) : param(p)  { };

    void Set(ParameterValue *param);

    char *StringValue();

    float Value();

    uint64 ValueNano();

    void KeyLeft();

    void KeyRight();

    void RegLeft();

    void RegRight();

    void PressKey(Control key);

    void SaveValue();

    ParameterValue *param;

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
    /// Возвращает число до запятой, выраженное в нано-единицах
    uint64 ValueBeforeCommaInNano();
    /// Возвращает число после запятой, выраженное в единицах нано-размерности
    uint64 ValueAfterCommaInNano();
    /// Переключает порядок на следующий по возрастанию
    void IncreaseOrder();
    /// Заслать текущее значение в генератор
    void SendToGenerator();
};



/// Окно редактирования численного значения параметра
class InputWindow
{
public:
    static void Init();

    static void Draw();

    static void OrderUp();

    static void OrderDown();

    static void KeyEnter();

    static void ProcessContorl(Control control);

private:

    static void KeyLeft();

    static void KeyRight();

    static void DrawAmplitude(int x, int y);

    static void DrawFrequency(int x, int y);

    static void DrawOffset(int x, int y);

    static void DrawPhase(int x, int y);

    static void DrawDelay(int x, int y);

    static void DrawPeriod(int x, int y);

    static void DrawDutyRatio(int x, int y);

    static void DrawDuration(int x, int y);

    static void DrawDigits(int x, int y);

    static void DrawDurationRise(int x, int y);

    static void DrawDurationFall(int x, int y);

    static void DrawDurationStady(int x, int y);

    static void DrawDutyFactor(int x, int y);

    static void DrawRampSineDuration(int x, int y);

    static void DrawRampSineAmplitude(int x, int y);

    static void DrawPacketPeriod(int x, int y);

    static void DrawPacketNumber(int x, int y);

    static void DrawExit(int x, int y);

    static ParameterValue *param;

    static StructValue iws;
};


#ifdef WIN32
#pragma warning(pop)
#endif
