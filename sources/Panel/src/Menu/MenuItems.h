// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Display/Text.h"
#include "Keyboard/Controls.h"
#include "MenuItemsDefs.h"
#include "common/Command.h"


typedef bool (*pFuncBKey)(const Control &);

class Button;
class Choice;
class ChoiceParameter;
class Page;
class SButton;


// ����� ����� ��� ���� ����� ��������� ����
#define COMMON_PART_MENU_ITEM                                                                                                                 \
    const TypeItem     type;          /* ��� ����� */                                                                                         \
    const int8         num;           /* ����� ��������� ��� Choice ��� ����� ��������� ��� Page*/                                            \
    const bool         isPageSB;      /* ���� true, �� ��� �������� ����� ������, ����� type == Item_Page */                                  \
    const uint8        name;          /* ��� �� ������������ NamePage, ���� type == Item_Page */                                              \
                                      /* � ������, ���� type == Item_Choice,  ���������� ����� ���� */                                        \
    const PageBase    *keeper;        /* ����� ��������, ������� �����������. ��� Page_Main = 0 */                                            \
    const char        *title[2];      /* �������� �������� �� ������� � ���������� ������ */                                                  \
    const char        *hint[2];       /* ��������� �� ������� � ���������� ������ */                                                          \
    pFuncBV           funcOfActive    /* ������� �� ������ ������� */

class PageBase;
class Page;
class Form;

// ������ ���� ������� ����
struct TypeItem
{
    enum E
    {
        NoneLight,
        NoneDark,
        Choice,             // ����� ������ - ��������� ������� ���� �� ���������� �������� ��������.
        Button,             // ������.
        Page,               // ��������.
        Governor,           // ��������� - ��������� ������� ����� ����� �������� �������� �� ������� ��������� ����������.
        GovernorColor,      // ��������� ������� ����.
        SmallButton,        // ������ ��� ������ ����� ������
        ChoiceParameter
    } value;

    TypeItem(E v) : value(v) {}

    bool IsButton() const          { return value == Button; }
    bool IsChoice() const          { return value == Choice; }
    bool IsChoiceParameter() const { return value == ChoiceParameter; }
    bool IsPage() const            { return value == Page; }
};


class Item
{
public:
    COMMON_PART_MENU_ITEM;
    
    // ���������� true, ���� ������� ���� control ������ (��������� �� �� ����� ������� ����. ��� �������, ��� ��������, ��� ������� �������������� ������� ���� ����� Choice ��� Governor
    bool IsShade() const;
    
    // ���������� true, ���� ������, ��������������� �������� ���� control, ��������� � ������� ���������
    bool IsPressed() const;
    
    // ���������� true, ���� ������� ���� �� ������ control ������
    bool IsOpened() const;

    void Open(bool open);
    
    // ���������� �������� ��������, ��� ��� �������� �� ������� �������
    String GetTitle() const;

    void Draw(bool opened, int x = -1, int y = -1) const;

    void DrawTitle(int x, int y) const;
    
    // ������������ ������� ������. ���������� ��������� �� ����, ���� ��������� � �������� ��������� ����� �������, � 0 � ��������� ������
    void Press(const Control &);

    // ���������� ���������� ����� ������ ���� �� ��������
    int PositionOnPage() const;
    
    // ���������� ������ ���� � �������� ����
    String FullPath();
    
    // ������� ��������� � �������� ����� ������. ���������� ������ ���������� ����������� ������
    int DrawHint(int x, int y, int width, const Color & = Color::NUMBER) const;

    Page *Keeper() const { return (Page *)keeper; }; //-V2567

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

// ��������� �������� ����.
class PageBase
{
public:
    COMMON_PART_MENU_ITEM;
    
    Item    **items;        // ����� ��������� �� ������ ���� �������� (� ������� ��������) ��� �������� ����� ������  ����� �������� 6 ���������� �� SButton : 0 - B_Menu, 1...5 - B_F1...B_F5
    pFuncVB   funcEnter;    // ����� ���������� ��� ������� �� �������� ��������
    pFuncVV   funcOnDraw;   // ����� ���������� ����� ��������� ������
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
    
    // ���������� true, ���� ������� ������� �������� ������
    bool CurrentItemIsOpened() const;
    
    // ����� ��������� �� ������ ���� �������� (� ������� ��������) ��� �������� ����� ������  ����� �������� 6 ���������� �� SButton : 
    // 0 - B_Menu,  1...5 - B_F1...B_F5
    const Item * const *items;   
    
    // ����� ���������� ��� ������� �� �������� ��������
    pFuncVB  funcEnter;
    
    // ����� ���������� ����� ��������� ������
    pFuncVV  funcOnDraw;
    
    // � �������� ����� ������ ���������� ��� �������� ����� ���������
    pFuncBKey funcOnKey;
    
    // D��������� ����� ���������� � �������� �� ������ page
    int NumSubPages() const;
    
    // ���������� ���������� ��������� � �������� �� ������ page
    int NumItems() const;
    
    // ���������� ����� ������� �����������
    int CurrentSubPage() const;
    
    // ������ �������� ��������
    void DrawOpened() const;
    
    // ������ �������� ��������
    void DrawClosed(int x, int y) const;

    void SetCurrentSubPage(int pos);
    
    // ������������� ������� ��������� ������ ����
    void SetPosActItem(int pos);
    
    // ���������� ����� �������� ����, ���������������� ������� �������������� �������
    Item *GetItem(const Key::E key) const;
    
    // ���������� ������� �������� �������� ������� page
    int8 PosCurrentItem() const;

    void ChangeSubPage(int delta);
    
    // ������������ ������� ������. ���������� ��������� �� ����, ���� ��������� � �������� ��������� ����� �������, � 0 � ��������� ������
    bool Press(const Control &);

    struct Title
    {
        static const int HEIGHT = 18;
    };

    static void FuncDraw() {};

    static void FuncEnter(bool) { };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Button ///

// ��������� ������.
class ButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    
    pFuncVV     funcOnPress;    // �������, ������� ���������� ��� ������� �� ������.
    pFuncVII    funcForDraw;    // ������� ����� ���������� �� ����� ��������� ������.
};

class Button : public Item
{
public:
    
    pFuncVV     funcOnPress;    // �������, ������� ���������� ��� ������� �� ������.
    pFuncVII    funcForDraw;    // ������� ����� ���������� �� ����� ��������� ������.
    
    // ������������ ������� ������. ���������� ����, ������ ��� �� ����� ���������� � �������� ���������.
    Item *Press(Action::E action);

    void Draw(int x, int y) const;

    static void FuncPress() { };
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SButton ///

struct StructHelpSmallButton
{
    pFuncVII    funcDrawUGO;    // ��������� �� ������� ��������� ����������� �������� ������
    cstr     helpUGO[2];     // ������� � ������� �����������.
};


// ��������� ������ ��� ��������������� ������ ����.
class SButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    
    pFuncVV                         funcOnPress;    // ��� ������� ��������� ��� ��������� ������� ������.
    pFuncVII                        funcForDraw;    // ��� ������� ���������� ��� ��������� ������ � ����� � ������������ x, y.
    const StructHelpSmallButton    *hintUGO;
    int                             numHints;
};


class SButton : public Item
{
public:
    
    pFuncVV                         funcOnPress;    // ��� ������� ��������� ��� ��������� ������� ������.
    pFuncVII                        funcForDraw;    // ��� ������� ���������� ��� ��������� ������ � ����� � ������������ x, y.
    const StructHelpSmallButton    *hintUGO; 
    int                             numHints;
    
    // ������������ ������� ������. ���������� 0, ������ ��� �� ����� ���������� � �������� ���������
    Item *Press(Action::E action);

    void Draw(int x, int y) const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///

// ��������� ���������.
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
    
    int16   minValue;       // ���������� ��������, ������� ����� ��������� ���������.
    int16   maxValue;       // ������������ ��������.
    int16  *cell;
    pFuncVV funcOfChanged;  // �������, ������� ����� �������� ����� ����, ��� �������� ���������� ����������.
    pFuncVV funcBeforeDraw; // �������, ������� ���������� ����� ����������
    pFuncVB funcPress;      // ������� ���������� ��� ������� �� item. �������� bool == true ��������, ��� item ��������������� ��������
    
    // ��������� �������� �������� ���������� ��� ���������� �������� ���� ���� Governor (� ����������� �� ����� delta).
    void StartChange(int detla);
    
    // ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 NextValue() const;
    
    // ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 PrevValue() const;
    
    // ������������ ��������� ���� ��������.
    float Step();
    
    // �������� �������� � ������� ������� ��� ��������� ��������.
    void ChangeValue(int delta);
    
    // ��� �������� �������� ������������ ������ �� ��������� �������.
    void NextPosition();
    
    // ���������� ����� ��������� � ���� ��� ����� �������� governor. ������� �� ������������� ��������, ������� ����� ��������� governor.
    int  NumDigits() const;
    
    // ���������� ����������� ����������, ��������������� ��� �������� ���������
    static char GetSymbol(int value);

    void Draw(int x, int y) const;

    void DrawValue(int x, int y) const;
    
    // ��������� �����������
    void Press(const Control &);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class ChoiceBase
{
public:
    COMMON_PART_MENU_ITEM;

    int8    *cell;
    cstr *namesRU;       // �������� ������ �� ������� � ���������� ������.
    cstr *namesEN;
    cstr *hintsRU;       // ��������� ��� ������� �������� �� ������� �����
    cstr *hintsEN;
    pFuncVB  funcOnChanged; // ������� ������ ���������� ����� ��������� �������� ��������.
    pFuncVII funcForDraw;   // ������� ���������� ����� ��������� ��������. 

    int8 CurrentIndex() const;
};

class Choice : public Item
{
public:

    int8 *cell;
    cstr *namesRU;       // �������� ������ �� ������� � ���������� ������.
    cstr *namesEN;
    cstr *hintsRU;       // ��������� ��� ������� �������� �� ������� �����
    cstr *hintsEN;
    pFuncVB  funcOnChanged; // ������� ������ ���������� ����� ��������� �������� ��������.
    pFuncVII funcForDraw;   // ������� ���������� ����� ��������� ��������. 

    void StartChange(int delta) const;
    
    // ������������ ��������� ���� ��������.
    float Step();
    
    // ���������� ���������� ��������� ������ � �������� �� ������ choice
    int8 NumSubItems() const;

    void Draw(bool opened, int x = -1, int y = -1) const;
    
    // ���������� ��� �������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������
    const String NameCurrentSubItem() const;
    
    // ���������� ��� ���������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������
    String NameNextSubItem() const;
    
    // ���������� ������ ����������
    int GetHeightOpened() const;

    pchar NamePrevSubItem();
    
    // ���������� ��� �������� ������ �������� choice � ������� i ��� ��� �������� � �������� ���� ���������
    String NameSubItem(int i) const;
    
    // ���������� ��������� �� ����, ���� ��������� � �������� ���������, � 0, ���� � ��������
    Item *Press(const Control &);

    int8 CurrentIndex() const;
};



class ChoiceParameterBase
{
public:
    COMMON_PART_MENU_ITEM;
    pFuncVV  funcOnPress;
    Form    *form;
};


class ChoiceParameter : public Item
{
public:
    pFuncVV  funcOnPress;
    Form    *form;
    
    // ������������ ������� ������. ���������� ��������� �� ����, ���� ��������� � �������� ��������� � 0 � ���������.
    Item *Press(const Control &);

    cstr NameSubItem(int num) const;

    String NameCurrentSubItem() const;

    void Draw(bool opened, int x, int y) const;

    // ���������� ������ ����������
    int GetHeightOpened() const;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Formula ////

// ��������� ������� ���� ��� ��������� ������������� � ������ �������������� �������
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
    
    int8 *function;         // ����� ������, ��� �������� Function, �� ������� ������ ���� ��������
    int8 *koeff1add;        // ����� ������������ ��� ������ ����� ��� ��������
    int8 *koeff2add;        // ����� ������������ ��� ������ ����� ��� ��������
    int8 *koeff1mul;        // ����� ������������ ��� ������ ����� ��� ���������
    int8 *koeff2mul;        // ����� ������������ ��� ������ ����� ��� ���������
    int8 *curDigit;         // ������� ������ : 0 - ���� ������� �����, 1 - ����������� ������� �����, 2 - ���� ������� �����, 3 - ����������� ������� �����
    pFuncVV funcOfChanged;  // ��� ������� ���������� ����� ��������� ��������� �������� ����������.
};

class Formula : public Item
{
public:
    
    int8 *function;         // ����� ������, ��� �������� Function, �� ������� ������ ���� ��������
    int8 *koeff1add;        // ����� ������������ ��� ������ ����� ��� ��������
    int8 *koeff2add;        // ����� ������������ ��� ������ ����� ��� ��������
    int8 *koeff1mul;        // ����� ������������ ��� ������ ����� ��� ���������
    int8 *koeff2mul;        // ����� ������������ ��� ������ ����� ��� ���������
    int8 *curDigit;         // ������� ������ : 0 - ���� ������� �����, 1 - ����������� ������� �����, 2 - ���� ������� �����, 3 - ����������� ������� �����
    pFuncVV funcOfChanged;  // ��� ������� ���������� ����� ��������� ��������� �������� ����������.
};
