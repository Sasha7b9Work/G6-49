#pragma once
#include "Hardware/Controls.h"
#include "Command.h"
#include "Menu/MenuItems.h"
#include <string.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Стуктура для обработки нажатия кнопок в окне ввода числового значения
class StructValue
{
public:

    StructValue(ParameterValue *p = 0) : param(p)  { };

    void Set(ParameterValue *param);

    char *StringValue();

    ParamValue ValueNano();

    void KeyLeft();

    void KeyRight();

    void RegLeft();

    void RegRight();

    void PressKey(Control &key);

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
    /// Восстановить значение параметра, если превышение offset + amplitude
    void SetParameterIfUnderLimit(ParameterValue &param);
};



/// Окно редактирования численного значения параметра
namespace InputWindow
{
    void Init();

    void DeInit();

    void Draw();

    void OrderUp();

    void OrderDown();

    void KeyEnter();

    void ProcessContorl(Control &control);
};
