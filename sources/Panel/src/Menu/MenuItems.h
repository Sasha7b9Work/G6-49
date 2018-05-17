#pragma once
#include "Display/Display.h"
#include "MenuItemsDefs.h"
#include "Menu/MenuPagesNames.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Menu
 *  @{
 *  @defgroup MenuItems Menu Items
 *  @{
 */

extern int8 gCurDigit;

#define MENU_ITEMS_ON_DISPLAY       5   ///< Сколько пунктов меню помещается на экране по вертикали.


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Разные виды пунктов меню
enum TypeControl
{
    Control_None,           
    Control_Choice,        ///< Пункт выбора - позволяет выбрать одно из нескольких заданных значений.
    Control_Button,        ///< Кнопка.
    Control_Page,          ///< Страница.
    Control_Governor,      ///< Регулятор - позволяет выбрать любое целое числовое значение из заранее заданного диапазаона.
    Control_GovernorColor, ///< Позволяет выбрать цвет.
    Control_ChoiceReg,     ///< Элемент выбора, в котором выбор осуществляется не кнопкой, а ручкой
    Control_SmallButton,   ///< Кнопка для режима малых кнопок
    Control_ChoiceParameter,
    Control_NumberItems
};


/// Общая часть для всех типов элементов меню
#define COMMON_PART_MENU_ITEM                                                                           \
    TypeControl     type;           /* Тип итема */                                                     \
    int8            num;            /* Число вариантов для Choice или число контролов для Page*/        \
    bool            isPageSB;       /* Если true, то это страница малых кнопок */                       \
    NamePage        name;           /* Имя из перечисления NamePage */                                  \
    const PageBase  *keeper;        /* Адрес страницы, которой принадлежит. Для Page_Main = 0 */        \
    pFuncBV         funcOfActive;   /* Активен ли данный элемент */                                     \
    const char      *titleHint[4]   /* Название страницы на русском и английском языках. Также подсказка для режима помощи */

class PageBase;

#define IS_PAGE(control)           (control->type == Control_Page)
#define NOT_PAGE(control)          (control->type != Control_Page)
#define IS_PAGE_SB(control)        (control->isPageSB)
#define IS_CHOICE(control)         (control->type == Control_Choice)
#define IS_CHOICE_REG(control)     (control->type == Control_ChoiceReg)
#define NOT_CHOICE_REG(control)    (control->type != Control_ChoiceReg)
#define IS_GOVERNOR(control)       (control->type == Control_Governor)
#define NOT_GOVERNOR(control)      (control->type != Control_Governor)
#define IS_GOVERNOR_COLOR(control) (control->type == Control_GovernorColor)
#define IS_IP(control)             (control->type == Control_IP)
#define IS_MAC(control)            (control->type == Control_MAC)
#define IS_TIME(control)           (control->type == Control_Time)
    
#define KEEPER(control)            ((PageBase *)control->keeper)
#define IS_ACTIVE(control)         (control->funcOfActive())


class Control
{
public:
    COMMON_PART_MENU_ITEM;
    /// \brief Возвращает true, если элемент меню control затенён (находится не на самом верхнем слое. Как правило, это означает, что раскрыт 
    /// раскрывающийся элемент меню вроде Choice или Governor
    bool IsShade() const;
    /// Возвращает true, если кнопка, соответствующая элементу меню control, находится в нажатом положении
    bool IsPressed() const;
    /// Сделать/разделать текущим
    void SetCurrent(bool active);
    /// Возвращает true, если элемент меню по адрему control открыт
    bool IsOpened() const;

    void Open(bool open);
    /// Возвращает название элемента, как оно выглядит на дисплее прибора
    const char *Title() const;

    bool ChangeOpened(int delta);
    /// Вызывается при "коротком" нажатии
    void ShortPress();
    /// Вызывается при "длинном" нажатии
    void LongPress();

    void Draw(bool opened, int x = -1, int y = -1) const;
    /// Обрабатывает нажатие кнопки. Возвращает указатель на себя, если находится в открытом состоянии после нажатия, и 0 в противном случае
    Control *Press(TypePress press);

    TypeControl Type() const;

    /// Возвращает порядковый номер пункта меню на странице
    int PositionOnPage() const;
    /// Возвращает функциональную клавишу, назначенную пункту меню
    PanelControl ButtonForItem() const;

    void Rotate(PanelControl control);
    /// Возвращает полный путь к элементу меню
    char *FullPath() const;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Page ///

/// Описывает страницу меню.
class PageBase
{
public:
    COMMON_PART_MENU_ITEM;
    /// \brief Здесь указатели на пункты этой страницы (в обычной странице) для страницы малых кнопок  здесь хранятся 6 указателей на SButton : 
    /// 0 - B_Menu, 1...5 - B_F1...B_F5
    const Control * const *items;                                               
    /// Будет вызываться при нажатии на свёрнутую страницу
    pFuncVV  funcOnPress;
    /// Будет вызываться после отрисовки кнопок
    pFuncVV  funcOnDraw;
    /// В странице малых кнопок вызывается при повороте ручки установка
    pFuncVI  funcRegSetSB;
    bool CurrentItemIsOpened() const;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
#define SMALL_BUTTON_FROM_PAGE(page, numButton)     ((SButton *)((Page *)page)->items[numButton])

class Page : public Control
{
public:
    /// Возвращает true, если текущий элемент страницы открыт
    bool CurrentItemIsOpened() const;
    /// \brief Здесь указатели на пункты этой страницы (в обычной странице) для страницы малых кнопок  здесь хранятся 6 указателей на SButton : 
    /// 0 - B_Menu,  1...5 - B_F1...B_F5
    const Control * const *items;   
    /// Будет вызываться при нажатии на свёрнутую страницу
    pFuncVV  funcOnPress;
    /// Будет вызываться после отрисовки кнопок
    pFuncVV  funcOnDraw;
    /// В странице малых кнопок вызывается при повороте ручки установка
    pFuncVI  funcRegSetSB;
    /// Dозвращает число подстраниц в странице по адресу page
    int NumSubPages() const;
    /// Возвращает количество элементов в странице по адресу page
    int NumItems() const;
    /// Возвращает имя страницы page
    NamePage GetNamePage() const;

    int8 CurrentSubPage() const;

    void SetCurrentSubPage(int8 pos);
    /// Устанавливает позицию активного пункта меню
    void SetPosActItem(int8 pos);
    /// Возвращает адрес элемента меню заданной страницы
    Control *Item(int numElement) const;
    /// \brief Возвращает позицию первого элемента страницы по адресу page на экране. Если текущая подстраница 0, это будет 0, если текущая 
    /// подстраница 1, это будет 5 и т.д.
    int PosItemOnTop() const;
    /// Вызывает функцию короткого нажатия кнопки над итемом numItem страницы page
    void ShortPressOnItem(int numItem);
    /// Возвращает позицию текущего элемента странцы page
    int8 PosCurrentItem() const;

    void ChangeSubPage(int delta);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Button ///

/// Описывает кнопку.
class ButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    /// Функция, которая вызывается при нажатии на кнопку.
    pFuncVV     funcOnPress;
    /// Функция будет вызываться во время отрисовки кнопки.
    pFuncVII    funcForDraw;
};

class Button : public Control
{
public:
    /// Функция, которая вызывается при нажатии на кнопку.
    pFuncVV     funcOnPress;
    /// Функция будет вызываться во время отрисовки кнопки.
    pFuncVII    funcForDraw;
    /// Обрабатывает нажатие кнопки. Возвращает ноль, потому что не может находиться в открытом состоянии.
    Control *Press(TypePress press);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SButton ///

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4623 4626 5027)
#endif
struct StructHelpSmallButton
{
    /// Указатель на функцию отрисовки изображения варианта кнопки
    pFuncVII    funcDrawUGO;
    /// Подпись к данному изображению.
    pString     helpUGO[2];
};
#ifdef WIN32
#pragma warning(pop)
#endif


/// Описывает кнопку для дополнительного режима меню.
class SButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    /// Эта функция вызвается для обработки нажатия кнопки.
    pFuncVV                         funcOnPress;
    /// Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    pFuncVII                        funcForDraw;

    const StructHelpSmallButton    *hintUGO;

    int                             numHints;
};


class SButton : public Control
{
public:
    /// Эта функция вызвается для обработки нажатия кнопки.
    pFuncVV                         funcOnPress;
    /// Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    pFuncVII                        funcForDraw;

    const StructHelpSmallButton    *hintUGO; 

    int                             numHints;
    /// Обрабатывает нажатие кнопки. Возвращает 0, потому что не может находиться в открытом состоянии
    Control *Press(TypePress press);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///

/// Описывает регулятор.
class GovernorBase
{
public:
    COMMON_PART_MENU_ITEM;
    /// Минмальное значение, которое может принимать регулятор.
    int16   minValue;
    /// Максимальное значение.
    int16   maxValue;

    int16  *cell;
    /// Функция, которую нужно вызывать после того, как значение регулятора изменилось.
    pFuncVV funcOfChanged;
    /// Функция, которая вызывается перед отрисовкой
    pFuncVV funcBeforeDraw;
};

class Governor : public Control
{
public:
    /// Минмальное значение, которое может принимать регулятор.
    int16   minValue;
    /// Максимальное значение.
    int16   maxValue;

    int16  *cell;
    /// Функция, которую нужно вызывать после того, как значение регулятора изменилось.
    pFuncVV funcOfChanged;
    /// Функция, которая вызывается перед отрисовкой
    pFuncVV funcBeforeDraw;
    /// Запускает процессс анимации инкремента или декремента элемента меню типа Governor (в зависимости от знака delta).
    void StartChange(int detla);
    /// Возвращает следующее большее значение, которое может принять governor.
    int16 NextValue() const;
    /// Возвращает следующее меньшее значение, которое может принять governor.
    int16 PrevValue() const;
    /// Рассчитывате следующий кадр анимации.
    float Step();
    /// Изменяет значение в текущей позиции при раскрытом элементе.
    void ChangeValue(int delta);
    /// При открытом элементе переставляет курсор на следующую позицию.
    void NextPosition();
    /// Возвращает число знакомест в поле для ввода элемента governor. Зависит от максимального значения, которое может принимать governor.
    int  NumDigits() const;
    /// Возвращает изображение регулятора, соответствующее его текущему положению
    static char GetSymbol(int value);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class ChoiceBase
{
public:
    COMMON_PART_MENU_ITEM;

    int8    *cell;
    /// Варианты выбора на русском и английском языках.
    pString *names;
    /// Функция должна вызываться после изменения значения элемента.
    pFuncVB	 funcOnChanged;
    /// Функция вызывается после отрисовки элемента. 
    pFuncVII funcForDraw;
};

class Choice : public Control
{
public:

    int8    *cell;
    /// Варианты выбора на русском и английском языках.
    pString *names;
    /// Функция должна вызываться после изменения значения элемента.
    pFuncVB	 funcOnChanged;
    /// Функция вызывается после отрисовки элемента. 
    pFuncVII funcForDraw;

    void  StartChange(int delta) const;
    /// Рассчитывает следующий кадр анимации.
    float Step();
    /// Изменяет значение choice в зависимости от величины и знака delta.
    void  ChangeIndex(int delta);
    /// Возвращает количество вариантов выбора в элементе по адресу choice
    int   NumSubItems() const;

    void  Draw(bool opened, int x = -1, int y = -1) const;
    /// Возвращает имя текущего варианта выбора элемента choice, как оно записано в исходном коде программы
    const char *NameCurrentSubItem() const;
    /// Возвращает имя следующего варианта выбора элемента choice, как оно записано в исходном коде программы
    const char *NameNextSubItem() const;
    /// Возвращает высоту раскрытого
    int GetHeightOpened() const;

    const char *NamePrevSubItem() const;
    /// Возвращает имя варианта выбора элемента choice в позиции i как оно записано в исходном коде программы
    const char *NameSubItem(int i) const;
    /// Возвращает указатель на себя, если находится ы открытом состоянии, и 0, если в закрытом
    Control *Press(TypePress press);

    int CurrentChoice() const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct AllowableParameters
{
    bool allow[NumParameters];
};

///
class ChoiceParameterBase
{
public:
    COMMON_PART_MENU_ITEM;
    pFuncVV              funcOnPress;
    AllowableParameters  allowParameters;
    uint8               *numParameter;
};


class ChoiceParameter : public Control
{
public:

    pFuncVV              funcOnPress;

    AllowableParameters  allowParameters;

    uint8               *numParameter;
    /// Обрабатывает нажатие кнопки. Возвращает указатель на себя, если находится в открытом состоянии и 0 в противном.
    Control *Press(TypePress press);

    pString NameSubItem(int num) const;

    pString CurrentName() const;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Formula ////

/// Описывает элемент меню для установки коэффициентов и знаков математической формулы
#define FIELD_SIGN_MEMBER_1_ADD 0
#define FIELD_SIGN_MEMBER_1_MUL 1
#define FIELD_SIGN_MEMBER_2_ADD 2
#define FIELD_SIGN_MEMBER_2_MUL 3
#define POS_SIGN_MEMBER_1       0
#define POS_KOEFF_MEMBER_1      1
#define POS_SIGN_MEMBER_2       2
#define POS_KOEFF_MEMBER_2      3

class FormulaBase
{
public:
    COMMON_PART_MENU_ITEM;
    /// Адрес ячейки, где хранится Function, из которой берётся знак операции
    int8 *function;
    /// Адрес коэффициента при первом члене для сложения
    int8 *koeff1add;
    /// Адрес коэффициента при втором члене для сложения
    int8 *koeff2add;
    /// Адрес коэффициента при первом члене для умножения
    int8 *koeff1mul;
    /// Адрес коэффициента при втором члене для умножения
    int8 *koeff2mul;
    /// Текущий разряд : 0 - знак первого члена, 1 - коэффициент первого члена, 2 - знак второго члена, 3 - коэффициент второго члена
    int8 *curDigit;
    /// Эта функция вызывается после изменения состояния элемента управления.
    pFuncVV funcOfChanged;
};

class Formula : public Control
{
public:
    /// Адрес ячейки, где хранится Function, из которой берётся знак операции
    int8 *function;
    /// Адрес коэффициента при первом члене для сложения
    int8 *koeff1add;
    /// Адрес коэффициента при втором члене для сложения
    int8 *koeff2add;
    /// Адрес коэффициента при первом члене для умножения
    int8 *koeff1mul;
    /// Адрес коэффициента при втором члене для умножения
    int8 *koeff2mul;
    /// Текущий разряд : 0 - знак первого члена, 1 - коэффициент первого члена, 2 - знак второго члена, 3 - коэффициент второго члена
    int8 *curDigit;
    /// Эта функция вызывается после изменения состояния элемента управления.
    pFuncVV funcOfChanged;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GovernorColor ///
class ColorType;

class GovernorColorBase
{
public:
    COMMON_PART_MENU_ITEM;
    /// Структура для описания цвета.
    ColorType *ct;
    /// Эту функцию нужно вызывать после изменения значения элемента.
    pFuncVV funcOnChanged;
};

class GovernorColor : public Control
{
public:
    /// Структура для описания цвета.
    ColorType *ct;
    /// Эту функцию нужно вызывать после изменения значения элемента.
    pFuncVV funcOnChanged;
    /// Изменить яркость цвета в governor.
    void ChangeValue(int delta);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Time ///

#define iEXIT   0
#define iDAY    1
#define iMONTH  2
#define iYEAR   3
#define iHOURS  4
#define iMIN    5
#define iSEC    6
#define iSET    7

class TimeBase
{
public:
    COMMON_PART_MENU_ITEM;
    /// Текущее поле установки. 0 - выход, 1 - сек, 2 - мин, 3 - часы, 4 - день, 5 - месяц, 6 - год, 7 - установить.
    int8 *curField;

    int8 *hours;

    int8 *minutes;

    int8 *seconds;

    int8 *month;

    int8 *day;

    int8 *year;
};

/// Устанавливает и показывает время.
class Time : public Control
{
public:
    /// Текущее поле установки. 0 - выход, 1 - сек, 2 - мин, 3 - часы, 4 - день, 5 - месяц, 6 - год, 7 - установить.
    int8 *curField;

    int8 *hours;

    int8 *minutes;

    int8 *seconds;

    int8 *month;

    int8 *day;

    int8 *year;

    void SetOpened();

    void IncCurrentPosition();

    void SetNewTime();

    void SelectNextPosition();

    void DecCurrentPosition();
};

#define CHOICE_RUN_FUNC_CHANGED(c, val)     \
    if(c->funcOnChanged)                    \
    {                                       \
        c->funcOnChanged(val);              \
    }


typedef void * pVOID;
#define MAX_NUM_ITEMS_IN_PAGE 15
typedef pVOID arrayItems[MAX_NUM_ITEMS_IN_PAGE];

const char* Parameter_Name(WaveParameter parameter);

/** @}  @}
 */
