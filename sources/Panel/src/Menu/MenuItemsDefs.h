#pragma once

inline void FuncDraw(int, int) {}
inline bool FuncOnKey(const Key&) { return false; }
inline void FuncChangedChoice(bool) {}
inline void FuncChanged() {}
inline void FuncBeforeDraw() {}
inline void EmptyFuncVV() {}
inline void EmptyFuncVB(bool) {}


#define DEF_BUTTON(name, titleRU, hintRU, titleEN, hintEN, keeper, funcActive, funcPress, funcDraw)                                        \
static const ButtonBase name = { TypeItem::Button, 0, false, Page::NoPage, &keeper, {titleRU, hintRU}, {titleEN, hintEN}, funcActive,    \
    funcPress, funcDraw };


#define DEF_GOVERNOR(name, titleRU, hintRU, titleEN, hintEN, cell, min, max, keeper, funcActive, funcChanged, funcBeforeDraw, funcPress)    \
static const GovernorBase name = { TypeItem::Governor, 0, false, Page::NoPage, (const PageBase *)&keeper,                                 \
    {titleRU, hintRU}, {titleEN, hintEN}, funcActive, min, max, (int16 *)(&(cell)), funcChanged, funcBeforeDraw, funcPress};


#define DEF_PAGE_SB(name, titleRU, hintRU, titleEN, hintEN, item0, item1, item2, item3,                                                     \
    namePage, keeper, funcActive, funcPress, funcDraw, funcOnKey)                                                                           \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item0),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item2),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item3)};                                                                                         \
const PageBase name = { TypeItem::Page, 4, true, namePage, keeper, {titleRU, hintRU}, {titleEN, hintEN}, funcActive,                      \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_1(name, titleRU, titleEN, hintRU, hintEN, item1, namePage, keeper, funcActive, funcPress)                                  \
static Item * items##name[] = {(Item *)&item1};                                                                                             \
const PageBase name = { TypeItem::Page, 1, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                       \
    items##name, funcPress, Page::FuncDraw, FuncRegSetPage};

#define DEF_PAGE_2(name, title, hint, item1, item2, namePage, keeper, funcActive, funcPress, funcOnKey, funcDraw)                           \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item2)};                                                                                         \
const PageBase name = { TypeItem::Page, 2, false, namePage, (const PageBase *)keeper, funcActive, {title, hint},                          \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_3(name, titleRU, hintRU, titleEN, hintEN, item1, item2, item3, namePage, keeper,                                           \
    funcActive, funcPress, funcOnKey, funcDraw)                                                                                             \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item2),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item3)};                                                                                         \
const PageBase name = { TypeItem::Page, 3, false, namePage, (const PageBase *)keeper, {titleRU, hintRU}, {titleEN, hintEN}, funcActive,   \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_4(name, titleRU, hintRU, titleEN, hintEN,                                                                                  \
    item1, item2, item3, item4, namePage, keeper, funcActive, funcPress, funcOnKey, funcDraw)                                               \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)item2,                                                                                                                          \
    (Item *)item3,                                                                                                                          \
    (Item *)item4};                                                                                                                         \
const PageBase name = { TypeItem::Page, 4, false, namePage, (const PageBase *)keeper, {titleRU, hintRU}, {titleEN, hintEN}, funcActive,   \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_4_VAR(name, titleRU, hintRU, titleEN, hintEN, item1, item2, item3, item4, namePage, keeper,                                \
    funcActive, funcPress, funcOnKey, funcDraw)                                                                                             \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(&item1),                                                                                         \
    (Item *)reinterpret_cast<const Item *>(&item2),                                                                                         \
    (Item *)reinterpret_cast<const Item *>(&item3),                                                                                         \
    (Item *)reinterpret_cast<const Item *>(&item4)};                                                                                        \
const PageBase name = { TypeItem::Page, 4, false, namePage, keeper, {titleRU, hintRU}, {titleEN, hintEN}, funcActive,                     \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_5(name, title, hint, item1, item2, item3, item4, item5, namePage, keeper, funcActive, funcPress, funcOnKey, funcDraw)      \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item2),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item3),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item4),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item5)};                                                                                         \
const PageBase name = { TypeItem::Page, 5, false, namePage, (const PageBase *)keeper, funcActive, {title, hint},                          \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_6(name, titleRU, hintRU, titleEN, hintEN, item1, item2, item3, item4, item5, item6, namePage, keeper,                      \
    funcActive, funcPress, funcOnKey, funcDraw)                                                                                             \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item2),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item3),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item4),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item5),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item6)};                                                                                         \
const PageBase name = { TypeItem::Page, 6, false, namePage, (const PageBase *)keeper, {titleRU, hintRU}, {titleEN, hintEN}, funcActive,   \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_7(name, title, hint,                                                                                                       \
    item1, item2, item3, item4, item5, item6, item7, namePage, keeper, funcActive, funcPress, funcOnKey, funcDraw)                          \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item2),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item3),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item4),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item5),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item6),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item7)};                                                                                         \
const PageBase name = { TypeItem::Page, 7, false, namePage, (const PageBase *)keeper, funcActive, {title, hint},                          \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_8(name, titleRU, hintRU, titleEN, hintEN,                                                                                  \
    item1, item2, item3, item4, item5, item6, item7, item8, namePage, keeper, funcActive, funcPress, funcOnKey, funcDraw)                   \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item2),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item3),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item4),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item5),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item6),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item7),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item8)};                                                                                         \
const PageBase name = { TypeItem::Page, 8, false, namePage, (const PageBase *)keeper, {titleRU, hintRU}, {titleEN, hintEN}, funcActive,   \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_9(name, title, hint,                                                                                                       \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, namePage, keeper, funcActive, funcPress, funcOnKey, funcDraw)            \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item2),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item3),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item4),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item5),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item6),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item7),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item8),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item9)};                                                                                         \
const PageBase name = { TypeItem::Page, 9, false, namePage, (const PageBase *)keeper, funcActive, {title, hint},                          \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_10(name, titleRU, titleEN, hintRU, hintEN,                                                                                 \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, namePage, keeper, funcActive, funcPress, funcOnKey)              \
static Item * items##name[] = {                                                                                                             \
    (Item *)&item1, (Item *)&item2, (Item *)&item3, (Item *)&item4, (Item *)&item5, (Item *)&item6, (Item *)&item7,                         \
    (Item *)&item8, (Item *)&item9, (Item *)&item10};                                                                                       \
const PageBase name = { TypeItem::Page, 10, false, namePage, (const PageBase *)keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},    \
    items##name, funcPress, FuncDrawPage, funcOnKey};

#define DEF_PAGE_11(name, titleRU, titleEN, hintRU, hintEN,                                                                                 \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, namePage, keeper, funcActive, funcPress, funcOnKey)      \
static Item * items##name[] = {                                                                                                             \
    (Item *)&item1, (Item *)&item2, (Item *)&item3, (Item *)&item4, (Item *)&item5, (Item *)&item6, (Item *)&item7,                         \
    (Item *)&item8, (Item *)&item9, (Item *)&item10, (Item *)&item11};                                                                      \
const PageBase name = { TypeItem::Page, 11, false, namePage, (const PageBase *)keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},    \
    items##name, funcPress, FuncDrawPage, funcOnKey};

#define DEF_PAGE_11_GLOBAL(name, titleRU, titleEN, hintRU, hintEN,                                                                          \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, namePage, keeper, funcActive, funcPress)                 \
static const Item * const  items##name[] = {(Item *)&item1, (Item *)&item2, (Item *)&item3, (Item *)&item4, (Item *)&item5,                 \
    (Item *)&item6, (Item *)&item7, (Item *)&item8, (Item *)&item9, (Item *)&item10, (Item *)&item11};                                      \
const PageBase name = { Item_Page, 11, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                             \
    items##name, funcPress, FuncDrawPage, FuncRegSetPage};

#define DEF_PAGE_12(name, titleRU, titleEN, hintRU, hintEN,                                                                                 \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12,                                                  \
    namePage, keeper, funcActive, funcPress, funcOnKey)                                                                                     \
static Item * items##name[] = {                                                                                                             \
    (Item *)&item1, (Item *)&item2, (Item *)&item3, (Item *)&item4, (Item *)&item5, (Item *)&item6, (Item *)&item7,                         \
    (Item *)&item8, (Item *)&item9, (Item *)&item10, (Item *)&item11, (Item *)&item12};                                                     \
const PageBase name = { TypeItem::Page, 12, false, namePage, (const PageBase *)keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},    \
    items##name, funcPress, FuncDrawPage, funcOnKey};

#define DEF_PAGE_13(name, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                        \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, item13)                                          \
static const Item * const  items##name[] = {                                                                                                \
    (Item *)&item1, (Item *)&item2, (Item *)&item3, (Item *)&item4, (Item *)&item5, (Item *)&item6, (Item *)&item7,                         \
    (Item *)&item8, (Item *)&item9, (Item *)&item10, (Item *)&item11, (Item *)&item12, (Item *)&item13};                                    \
const PageBase name = { Item_Page, 13, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                             \
    items##name, funcPress, FuncDrawPage, FuncRegSetPage};

#define DEF_PAGE_15(name, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                        \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10,                                                                  \
    item11, item12, item13, item14, item15)                                                                                                 \
static const Item * const  items##name[] = {(Item *)&item1, (Item *)&item2, (Item *)&item3, (Item *)&item4, (Item *)&item5,                 \
    (Item *)&item6, (Item *)&item7, (Item *)&item8, (Item *)&item9, (Item *)&item10, (Item *)&item11, (Item *)&item12,                      \
    (Item *)&item13, (Item *)&item14,                                                                                                       \
    (Item *)&item15};                                                                                                                       \
const PageBase name = { Item_Page, 15, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                             \
    items##name, funcPress, FuncDrawPage, FuncRegSetPage};



#define DEF_CHOICE_PARAMETER(name, titleRU, hintRU, titleEN, hintEN, keeper, funcActive, funcPress, form)                                   \
ChoiceParameterBase name = { TypeItem::ChoiceParameter, 0, false, Page::NoPage, &keeper, {titleRU, hintRU}, {titleEN, hintEN}, funcActive,\
    funcPress, form};



#define DEF_SMALL_BUTTON(name, titleRU, hintRU, titleEN, hintEN, keeper, funcActive, funcPress, funcDraw)                                                        \
static const SButtonBase name = { TypeItem::SmallButton, 0, false, Page::NoPage,                                                          \
    reinterpret_cast<const PageBase *>(const_cast<const Page*>(&keeper)),                                                                   \
    {titleRU, hintRU}, {titleRU, hintEN}, funcActive, funcPress, funcDraw, 0, 0};

#define DEF_SMALL_BUTTON_EXIT(name, keeper, funcActive, funcPress, funcDraw)                                                                \
static const SButtonBase name = { Item_SmallButton, 0, false, Page::NoPage, &keeper, funcActive, {"Выход", "Exit",                          \
    "Кнопка для выхода в предыдущее меню", "Button to return to the previous menu"}, funcPress, funcDraw, 0, 0};

#define DEF_SMALL_BUTTON_HINTS_2(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, funcPress, funcDraw,                           \
    FuncDrawHint1, hintRu1, hintEn1, FuncDrawHint2, hintRu2, hintEn2)                                                                       \
static const StructHelpSmallButton hints##name[] = {{ FuncDrawHint1, hintRu1, hintEn1 }, { FuncDrawHint2, hintRu2, hintEn2 }};              \
static const SButtonBase name = { Item_SmallButton, 0, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},        \
    funcPress, funcDraw, hints##name, 2};

#define DEF_SMALL_BUTTON_HINTS_3(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, funcPress, funcDraw,                           \
    FuncDrawHint1, hintRu1, hintEn1, FuncDrawHint2, hintRu2, hintEn2, FuncDrawHint3, hintRu3, hintEn3)                                      \
static const StructHelpSmallButton hints##name[] = {{ FuncDrawHint1, hintRu1, hintEn1 }, { FuncDrawHint2, hintRu2, hintEn2 },               \
    { FuncDrawHint3, hintRu3, hintEn3 } };                                                                                                  \
static const SButtonBase name = { Item_SmallButton, 0, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},        \
    funcPress, funcDraw, hints##name, 3};

#define DEF_SMALL_BUTTON_HINTS_5(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, funcPress, funcDraw,                           \
    FuncDrawHint1, hintRu1, hintEn1, FuncDrawHint2, hintRu2, hintEn2, FuncDrawHint3, hintRu3, hintEn3,                                      \
    FuncDrawHint4, hintRu4, hintEn4, FuncDrawHint5, hintRu5, hintEn5)                                                                       \
static const StructHelpSmallButton hints##name[] = {{ FuncDrawHint1, hintRu1, hintEn1 }, { FuncDrawHint2, hintRu2, hintEn2 },               \
    { FuncDrawHint3, hintRu3, hintEn3 }, { FuncDrawHint4, hintRu4, hintEn4 }, { FuncDrawHint5, hintRu5, hintEn5 } };                        \
static const SButtonBase name = { Item_SmallButton, 0, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},        \
    funcPress, funcDraw, hints##name, 5};


#define DEF_CHOICE_2(name, titleRU, hintRU, titleEN, hintEN,                                                                                        \
    name1RU, name1EN, hint1RU, hint1EN, name2RU, name2EN, hint2RU, hint2EN, cell, keeper, funcActive, funcChanged, funcDraw)                        \
static pString name##namesRU[] = {name1RU, name2RU};                                                                                                \
static pString name##namesEN[] = {name1EN, name2EN};                                                                                                \
static pString name##hintsRU[] = {hint1RU, hint2RU};                                                                                                \
static pString name##hintsEN[] = {hint1EN, hint2EN};                                                                                                \
static const ChoiceBase name = { TypeItem::Choice, 2, false, Page::NoPage, (const PageBase *)(&keeper),                                           \
    {titleRU, hintRU}, {titleEN, hintEN}, funcActive, (int8 *)&cell,                                                                                \
    name##namesRU, name##namesEN, name##hintsRU, name##hintsEN, funcChanged, funcDraw};

#define DEF_CHOICE_3(name, titleRU, hintRU, titleEN, hintEN,                                                                                        \
    name1RU, name1EN, hint1RU, hint1EN, name2RU, name2EN, hint2RU, hint2EN, name3RU, name3EN, hint3RU, hint3EN, cell, keeper,                       \
    funcActive, funcChanged, funcDraw)                                                                                                              \
static pString name##namesRU[] = {name1RU, name2RU, name3RU};                                                                                       \
static pString name##namesEN[] = {name1EN, name2EN, name3EN};                                                                                       \
static pString name##hintsRU[] = {hint1RU, hint2RU, hint3RU};                                                                                       \
static pString name##hintsEN[] = {hint1EN, hint2EN, hint3EN};                                                                                       \
static const ChoiceBase name = { TypeItem::Choice, 3, false, Page::NoPage, &keeper, {titleRU, hintRU}, {titleEN, hintEN}, funcActive,             \
    (int8 *)&cell, name##namesRU, name##namesEN, name##hintsRU, name##hintsEN, funcChanged, funcDraw};

#define DEF_CHOICE_4(name, titleRU, hintRU, titleEN, hintEN,                                                                                        \
    name1RU, name1EN, hint1RU, hint1EN, name2RU, name2EN, hint2RU, hint2EN, name3RU, name3EN, hint3RU, hint3EN, name4RU, name4EN, hint4RU, hint4EN, \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                \
static pString name##namesRU[] = {name1RU, name2RU, name3RU, name4RU};                                                                              \
static pString name##namesEN[] = {name1EN, name2EN, name3EN, name4EN};                                                                              \
static pString name##hintsRU[] = {hint1RU, hint2RU, hint3RU, hint4RU};                                                                              \
static pString name##hintsEN[] = {hint1EN, hint2EN, hint3EN, hint4EN};                                                                              \
static const ChoiceBase name = { TypeItem::Choice, 4, false, Page::NoPage, &keeper, {titleRU, hintRU}, {titleEN, hintEN}, funcActive,             \
    (int8 *)&cell, name##namesRU, name##namesEN, name##hintsRU, name##hintsEN, funcChanged, funcDraw};

#define DEF_CHOICE_5(name, titleRU, hintRU, titleEN, hintEN,                                                                                        \
    name1RU, name1EN, hint1RU, hint1EN, name2RU, name2EN, hint2RU, hint2EN, name3RU, name3EN, hint3RU, hint3EN, name4RU, name4EN, hint4RU, hint4EN, \
    name5RU, name5EN, hint5RU, hint5EN, cell, keeper, funcActive, funcChanged, funcDraw)                                                            \
static pString name##namesRU[] = {name1RU, name2RU, name3RU, name4RU, name5RU};                                                                     \
static pString name##namesEN[] = {name1EN, name2EN, name3EN, name4EN, name5EN};                                                                     \
static pString name##hintsRU[] = {hint1RU, hint2RU, hint3RU, hint4RU, hint5RU};                                                                     \
static pString name##hintsEN[] = {hint1EN, hint2EN, hint3EN, hint4EN, hint5EN};                                                                     \
static const ChoiceBase name = { TypeItem::Choice, 5, false, Page::NoPage, &keeper, {titleRU, hintRU}, {titleEN, hintEN}, funcActive,             \
    (int8 *)&cell, name##namesRU, name##namesEN, name##hintsRU, name##hintsEN, funcChanged, funcDraw};

#define DEF_CHOICE_6(name, title, hint, name1, hint1, name2, hint2, name3, hint3, name4, hint4, name5, hint5, name6, hint6,                         \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6};                                                                          \
static pString hints##name##Ru[] = {hint1, hint2, hint3, hint4, hint5, hint6};                                                                      \
static const ChoiceBase name = { TypeItem::Choice, 6, false, Page::NoPage, &keeper, funcActive, {title, hint},                                    \
    (int8 *)&cell, hints##name, hints##name##Ru, funcChanged, funcDraw};

#define DEF_CHOICE_7(name, titleRU, hintRU, titleEN, hintEN,                                                                                        \
    name1RU, name1EN, hint1RU, hint1EN, name2RU, name2EN, hint2RU, hint2EN, name3RU, name3EN, hint3RU, hint3EN, name4RU, name4EN, hint4RU, hint4EN, \
    name5RU, name5EN, hint5RU, hint5EN, name6RU, name6EN, hint6RU, hint6EN, name7RU, name7EN, hint7RU, hint7EN,                                     \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                \
static pString name##namesRU[] = {name1RU, name2RU, name3RU, name4RU, name5RU, name6RU, name7RU};                                                   \
static pString name##namesEN[] = {name1EN, name2EN, name3EN, name4EN, name5EN, name6EN, name7EN};                                                   \
static pString name##hintsRU[] = {hint1RU, hint2RU, hint3RU, hint4RU, hint5RU, hint6RU, hint7RU};                                                   \
static pString name##hintsEN[] = {hint1EN, hint2EN, hint3EN, hint4EN, hint5EN, hint6EN, hint7EN};                                                   \
static const ChoiceBase name = { TypeItem::Choice, 7, false, Page::NoPage, &keeper, {titleRU, hintRU}, {titleEN, hintEN}, funcActive,             \
    (int8 *)&cell, name##namesRU, name##namesEN, name##hintsRU, name##hintsEN, funcChanged, funcDraw};

#define DEF_CHOICE_8(name, titleRU, hintRU, titleEN, hintEN,                                                                                        \
    name1RU, name1EN, hint1RU, hint1EN, name2RU, name2EN, hint2RU, hint2EN, name3RU, name3EN, hint3RU, hint3EN, name4RU, name4EN, hint4RU, hint4EN, \
    name5RU, name5EN, hint5RU, hint5EN, name6RU, name6EN, hint6RU, hint6EN, name7RU, name7EN, hint7RU, hint7EN, name8RU, name8EN, hint8RU, hint8EN, \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                \
static pString name##namesRU[] = {name1RU, name2RU, name3RU, name4RU, name5RU, name6RU, name7RU, name8RU};                                          \
static pString name##namesEN[] = {name1EN, name2EN, name3EN, name4EN, name5EN, name6EN, name7EN, name8EN};                                          \
static pString name##hintsRU[] = {hint1RU, hint2RU, hint3RU, hint4RU, hint5RU, hint6RU, hint7RU, hint8RU};                                          \
static pString name##hintsEN[] = {hint1EN, hint2EN, hint3EN, hint4EN, hint5EN, hint6EN, hint7EN, hint8EN};                                          \
static const ChoiceBase name = { TypeItem::Choice, 8, false, Page::NoPage, &keeper, {titleRU, hintRU}, {titleEN, hintEN}, funcActive,             \
    (int8 *)&cell, name##namesRU, name##namesEN, name##hintsRU, name##hintsEN, funcChanged, funcDraw};

#define DEF_CHOICE_12(name, titleRU, titleEN, hintRU, hintEN,                                                                                       \
    nameRu1,  nameEn1,  hintRu1,  hintEn1,  nameRu2,  nameEn2,  hintRu2,  hintEn2,  nameRu3,  nameEn3,  hintRu3,  hintEn3,                          \
    nameRu4,  nameEn4,  hintRu4,  hintEn4,  nameRu5,  nameEn5,  hintRu5,  hintEn5,  nameRu6,  nameEn6,  hintRu6,  hintEn6,                          \
    nameRu7,  nameEn7,  hintRu7,  hintEn7,  nameRu8,  nameEn8,  hintRu8,  hintEn8,  nameRu9,  nameEn9,  hintRu9,  hintEn9,                          \
    nameRu10, nameEn10, hintRu10, hintEn10, nameRu11, nameEn11, hintRu11, hintEn11, nameRu12, nameEn12, hintRu12, hintEn12,                         \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,                                             \
    nameRu5, nameEn5, nameRu6,  nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10, nameRu11, nameEn11,              \
    nameRu12, nameEn12};                                                                                                                            \
static pString hints##name##Ru[] = {hintRu1, hintRu2, hintRu3, hintRu4, hintRu5, hintRu6, hintRu7, hintRu8, hintRu9, hintRu10, hintRu11, hintRu12}; \
static pString hints##name##En[] = {hintEn1, hintEn2, hintEn3, hintEn4, hintEn5, hintEn6, hintEn7, hintEn8, hintEn9, hintEn10, hintEn11, hintEn12}; \
static const ChoiceBase name = { TypeItem::Choice, 12, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},              \
    (int8 *)&cell, hints##name, hints##name##Ru, hints##name##En, funcChanged, funcDraw};


#define DEF_CHOICE_REG_9(name, titleRU, titleEN, hintRU, hintEN,                                                                                    \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5,                                                       \
    nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9,                                                                         \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,                                             \
    nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9};                                                      \
static const ChoiceBase name = { Item_ChoiceReg, 9, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                   \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_REG_10(name, titleRU, titleEN, hintRU, hintEN,                                                                                   \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5,                                                       \
    nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10,                                                     \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,                                             \
    nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10};                                  \
static const ChoiceBase name = { Item_ChoiceReg, 10, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                  \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};
