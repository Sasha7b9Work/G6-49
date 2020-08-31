#pragma once
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Display/Text.h"
#include "Keyboard/Controls.h"
#include "MenuItemsDefs.h"
#include "common/Command.h"


typedef bool (*pFuncBKey)(const Control);


// Общая часть для всех типов элементов меню
#define COMMON_PART_MENU_ITEM                                                                                                               \
    TypeItem::E     type;           /* Тип итема */                                                                                         \
    int8            num;            /* Число вариантов для Choice или число контролов для Page*/                                            \
    bool            isPageSB;       /* Если true, то это страница малых кнопок, когда type == Item_Page */                                  \
    uint8           name;           /* Имя из перечисления NamePage, если type == Item_Page */                                              \
                                    /* В случае, если type == Item_Choice,  определяет номер бита */                                        \
    const PageBase  *keeper;        /* Адрес страницы, которой принадлежит. Для Page_Main = 0 */                                            \
    const char      *title[2];      /* Название страницы на русском и английском языках */                                                  \
    const char      *hint[2];       /* Подсказка на русском и английском языках */                                                          \
    pFuncBV         funcOfActive    /* Активен ли данный элемент */

class PageBase;
class Page;
class Form;

// Разные виды пунктов меню
struct TypeItem
{
    enum E
    {
        NoneLight,
        NoneDark,
        Choice,             // Пункт выбора - позволяет выбрать одно из нескольких заданных значений.
        Button,             // Кнопка.
        Page,               // Страница.
        Governor,           // Регулятор - позволяет выбрать любое целое числовое значение из заранее заданного диапазаона.
        GovernorColor,      // Позволяет выбрать цвет.
        SmallButton,        // Кнопка для режима малых кнопок
        ChoiceParameter
    };
};


   
class Item
{
public:
    COMMON_PART_MENU_ITEM;
    
    // Возвращает true, если элемент меню control затенён (находится не на самом верхнем слое. Как правило, это означает, что раскрыт раскрывающийся элемент меню вроде Choice или Governor
    bool IsShade() const;
    
    // Возвращает true, если кнопка, соответствующая элементу меню control, находится в нажатом положении
    bool IsPressed() const;
    
    // Возвращает true, если элемент меню по адрему control открыт
    bool IsOpened() const;

    void Open(bool open);
    
    // Возвращает название элемента, как оно выглядит на дисплее прибора
    String GetTitle() const;

    void Draw(bool opened, int x = -1, int y = -1) const;

    void DrawTitle(int x, int y) const;
    
    // Обрабатывает нажатие кнопки. Возвращает указатель на себя, если находится в открытом состоянии после нажатия, и 0 в противном случае
    void Press(const Control control);

    TypeItem::E GetType() const;

    // Возвращает порядковый номер пункта меню на странице
    int PositionOnPage() const;
    
    // Возвращает полный путь к элементу меню
    String FullPath();
    
    // Вывести подсказку в заданном месте экрана. Возвращает нижнюю координату выведенного тектса
    int DrawHint(int x, int y, int width, Color color = Color::NUMBER) const;

    Page *Keeper() const { return const_cast<Page *>(reinterpret_cast<const Page *>(keeper)); }; //-V2567

    bool IsActive() const { return funcOfActive(); };

    static const int WIDTH = 69;

    static const int HEIGHT = 55;

    struct Title
    {
        static const int HEIGHT = 12;
    };

    static bool FuncActive() { return true; }

    static Item emptyDark;
    static Item emptyLight;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Page ///

// Описывает страницу меню.
class PageBase
{
public:
    COMMON_PART_MENU_ITEM;
    
    Item **items;           // Здесь указатели на пункты этой страницы (в обычной странице) для страницы малых кнопок  здесь хранятся 6 указателей на SButton : 0 - B_Menu, 1...5 - B_F1...B_F5
    pFuncVB funcEnter;      // Будет вызываться при нажатии на свёрнутую страницу
    pFuncVV funcOnDraw;     // Будет вызываться после отрисовки кнопок
    pFuncBKey funcOnKey;

    bool CurrentItemIsOpened() const;
};


#define SMALL_BUTTON_FROM_PAGE(page, numButton)     ((SButton *)((Page *)page)->items[numButton])

class Page : public Item
{
public:

    enum E
    {
         NoPage,
         Service,
         Debug,
         FrequencyCounter,
         USB,
         SB_Input,
         Registers,
         Signals,
         Main,
         Settings_Load,
         Settings_CalibrationA,
         Settings_CalibrationB,
         Calibration,
         SubRanges,
         Colors,
         Color_ChanA,
         Color_ChanB,
         Color_Menu,
         Count
    };
    
    // Возвращает true, если текущий элемент страницы открыт
    bool CurrentItemIsOpened() const;
    
    // Здесь указатели на пункты этой страницы (в обычной странице) для страницы малых кнопок  здесь хранятся 6 указателей на SButton : 
    // 0 - B_Menu,  1...5 - B_F1...B_F5
    const Item * const *items;   
    
    // Будет вызываться при нажатии на свёрнутую страницу
    pFuncVB  funcEnter;
    
    // Будет вызываться после отрисовки кнопок
    pFuncVV  funcOnDraw;
    
    // В странице малых кнопок вызывается при повороте ручки установка
    pFuncBKey funcOnKey;
    
    // Dозвращает число подстраниц в странице по адресу page
    int NumSubPages() const;
    
    // Возвращает количество элементов в странице по адресу page
    int NumItems() const;
    
    // Возвращает номер текущей подстраницы
    int CurrentSubPage() const;
    
    // Рисует открытую страницу
    void DrawOpened() const;
    
    // Рисует закрытую страницу
    void DrawClosed(int x, int y) const;

    void SetCurrentSubPage(int pos);
    
    // Устанавливает позицию активного пункта меню
    void SetPosActItem(int pos);
    
    // Возвращает адрес элемента меню, соответствующего данного функциональной клавише
    Item *GetItem(const Key::E key) const;
    
    // Возвращает позицию текущего элемента странцы page
    int8 PosCurrentItem() const;

    void ChangeSubPage(int delta);
    
    // Обрабатывает нажатие кнопки. Возвращает указатель на себя, если находится в открытом состоянии после нажатия, и 0 в противном случае
    bool Press(const Control control);

    struct Title
    {
        static const int HEIGHT = 18;
    };

    static void FuncDraw() {};

    static void FuncEnter(bool) { };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Button ///

// Описывает кнопку.
class ButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    
    pFuncVV     funcOnPress;    // Функция, которая вызывается при нажатии на кнопку.
    pFuncVII    funcForDraw;    // Функция будет вызываться во время отрисовки кнопки.
};

class Button : public Item
{
public:
    
    pFuncVV     funcOnPress;    // Функция, которая вызывается при нажатии на кнопку.
    pFuncVII    funcForDraw;    // Функция будет вызываться во время отрисовки кнопки.
    
    // Обрабатывает нажатие кнопки. Возвращает ноль, потому что не может находиться в открытом состоянии.
    Item *Press(Action::E action);

    void Draw(int x, int y) const;

    static void FuncPress() { };
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SButton ///

struct StructHelpSmallButton
{
    pFuncVII    funcDrawUGO;    // Указатель на функцию отрисовки изображения варианта кнопки
    pString     helpUGO[2];     // Подпись к данному изображению.
};


// Описывает кнопку для дополнительного режима меню.
class SButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    
    pFuncVV                         funcOnPress;    // Эта функция вызвается для обработки нажатия кнопки.
    pFuncVII                        funcForDraw;    // Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    const StructHelpSmallButton    *hintUGO;
    int                             numHints;
};


class SButton : public Item
{
public:
    
    pFuncVV                         funcOnPress;    // Эта функция вызвается для обработки нажатия кнопки.
    pFuncVII                        funcForDraw;    // Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    const StructHelpSmallButton    *hintUGO; 
    int                             numHints;
    
    // Обрабатывает нажатие кнопки. Возвращает 0, потому что не может находиться в открытом состоянии
    Item *Press(Action::E action);

    void Draw(int x, int y) const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///

// Описывает регулятор.
class GovernorBase
{
public:
    COMMON_PART_MENU_ITEM;
    int16   minValue;
    int16   maxValue;
    int16  *cell;
    pFuncVV funcOfChanged;
    pFuncVV funcBeforeDraw;
    pFuncVB funcPress;
};

class Governor : public Item
{
public:
    
    int16   minValue;       // Минмальное значение, которое может принимать регулятор.
    int16   maxValue;       // Максимальное значение.
    int16  *cell;
    pFuncVV funcOfChanged;  // Функция, которую нужно вызывать после того, как значение регулятора изменилось.
    pFuncVV funcBeforeDraw; // Функция, которая вызывается перед отрисовкой
    pFuncVB funcPress;      // Функция вызывается при нажатие на item. Параметр bool == true означает, что item устанавливается активным
    
    // Запускает процессс анимации инкремента или декремента элемента меню типа Governor (в зависимости от знака delta).
    void StartChange(int detla);
    
    // Возвращает следующее большее значение, которое может принять governor.
    int16 NextValue() const;
    
    // Возвращает следующее меньшее значение, которое может принять governor.
    int16 PrevValue() const;
    
    // Рассчитывате следующий кадр анимации.
    float Step();
    
    // Изменяет значение в текущей позиции при раскрытом элементе.
    void ChangeValue(int delta);
    
    // При открытом элементе переставляет курсор на следующую позицию.
    void NextPosition();
    
    // Возвращает число знакомест в поле для ввода элемента governor. Зависит от максимального значения, которое может принимать governor.
    int  NumDigits() const;
    
    // Возвращает изображение регулятора, соответствующее его текущему положению
    static char GetSymbol(int value);

    void Draw(int x, int y) const;

    void DrawValue(int x, int y) const;
    
    // Обработка воздействия
    void Press(const Control control);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class ChoiceBase
{
public:
    COMMON_PART_MENU_ITEM;

    int8    *cell;
    pString *namesRU;       // Варианты выбора на русском и английском языках.
    pString *namesEN;
    pString *hintsRU;       // Подсказки для каждого варианта на русском языке
    pString *hintsEN;
    pFuncVB	 funcOnChanged; // Функция должна вызываться после изменения значения элемента.
    pFuncVII funcForDraw;   // Функция вызывается после отрисовки элемента. 

    int8 CurrentIndex() const;
};

class Choice : public Item
{
public:

    int8 *cell;
    pString *namesRU;       // Варианты выбора на русском и английском языках.
    pString *namesEN;
    pString *hintsRU;       // Подсказки для каждого варианта на русском языке
    pString *hintsEN;
    pFuncVB	 funcOnChanged; // Функция должна вызываться после изменения значения элемента.
    pFuncVII funcForDraw;   // Функция вызывается после отрисовки элемента. 

    void StartChange(int delta) const;
    
    // Рассчитывает следующий кадр анимации.
    float Step();
    
    // Возвращает количество вариантов выбора в элементе по адресу choice
    int8 NumSubItems() const;

    void Draw(bool opened, int x = -1, int y = -1) const;
    
    // Возвращает имя текущего варианта выбора элемента choice, как оно записано в исходном коде программы
    const String NameCurrentSubItem() const;
    
    // Возвращает имя следующего варианта выбора элемента choice, как оно записано в исходном коде программы
    String NameNextSubItem() const;
    
    // Возвращает высоту раскрытого
    int GetHeightOpened() const;

    const char *NamePrevSubItem();
    
    // Возвращает имя варианта выбора элемента choice в позиции i как оно записано в исходном коде программы
    String NameSubItem(int i) const;
    
    // Возвращает указатель на себя, если находится ы открытом состоянии, и 0, если в закрытом
    Item *Press(const Control control);

    int8 CurrentIndex() const;
};



class ChoiceParameterBase
{
public:
    COMMON_PART_MENU_ITEM;
    pFuncVV     funcOnPress;
    Form  *form;
};


class ChoiceParameter : public Item
{
public:
    pFuncVV     funcOnPress;
    Form  *form;
    
    // Обрабатывает нажатие кнопки. Возвращает указатель на себя, если находится в открытом состоянии и 0 в противном.
    Item *Press(const Control control);

    pString NameSubItem(int num) const;

    String NameCurrentSubItem() const;

    void Draw(bool opened, int x, int y) const;

    // Возвращает высоту раскрытого
    int GetHeightOpened() const;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Formula ////

// Описывает элемент меню для установки коэффициентов и знаков математической формулы
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
    
    int8 *function;         // Адрес ячейки, где хранится Function, из которой берётся знак операции
    int8 *koeff1add;        // Адрес коэффициента при первом члене для сложения
    int8 *koeff2add;        // Адрес коэффициента при втором члене для сложения
    int8 *koeff1mul;        // Адрес коэффициента при первом члене для умножения
    int8 *koeff2mul;        // Адрес коэффициента при втором члене для умножения
    int8 *curDigit;         // Текущий разряд : 0 - знак первого члена, 1 - коэффициент первого члена, 2 - знак второго члена, 3 - коэффициент второго члена
    pFuncVV funcOfChanged;  // Эта функция вызывается после изменения состояния элемента управления.
};

class Formula : public Item
{
public:
    
    int8 *function;         // Адрес ячейки, где хранится Function, из которой берётся знак операции
    int8 *koeff1add;        // Адрес коэффициента при первом члене для сложения
    int8 *koeff2add;        // Адрес коэффициента при втором члене для сложения
    int8 *koeff1mul;        // Адрес коэффициента при первом члене для умножения
    int8 *koeff2mul;        // Адрес коэффициента при втором члене для умножения
    int8 *curDigit;         // Текущий разряд : 0 - знак первого члена, 1 - коэффициент первого члена, 2 - знак второго члена, 3 - коэффициент второго члена
    pFuncVV funcOfChanged;  // Эта функция вызывается после изменения состояния элемента управления.
};
