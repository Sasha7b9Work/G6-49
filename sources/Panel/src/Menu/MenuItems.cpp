#include "MenuItems.h"
#include "Menu.h"
#include "Hardware/Controls.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char *nameParameter[NumParameters][2] =
{
    {"×ÀÑÒÎÒÀ",         "FREQUENCY"},
    {"ÏÅÐÈÎÄ",          "PERIOD"},
    {"ÀÌÏËÈÒÓÄÀ",       "AMPLITUDE"},
    {"ÑÌÅÙÅÍÈÅ",        "OFFSET"},
    {"ÄËÈÒÅËÜÍÎÑÒÜ",    "DURATION"},
    {"ÑÊÂÀÆÍÎÑÒÜ",      "DUTY RATIO"},
    {"ÔÀÇÀ",            "PHASE"},
    {"ÇÀÄÅÐÆÊÀ",        "DELAY"}
};

static const char *nameWaveForm[NumForms][2] =
{
    {"ÑÈÍÓÑ",    "SINE"},
    {"ÏÈËÀ",     "SAW"},
    {"ÈÌÏÓËÜÑ",  "IMPULSE"}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *ItemTitle(void* item)
{
    return ((Choice*)item)->title[LANGUAGE];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void* ItemFromPage(int numItem)
{
    return CurrentPage()->items[numItem];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice_CurrentName(Choice* choice)
{
    return choice->names[(*(choice->cell)) * 2 + LANGUAGE];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *ChoiceWaveParameter_CurrentName(ChoiceParameter *choice)
{
    return (char*)(nameParameter[*(choice->numParameter)][LANGUAGE]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char* Page_Name(const Page* page)
{
    return page->title[LANGUAGE];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ItemIsChoice(void* item)
{
    return item && (TypeItem(item) == Item_Choice);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ItemIsChoiceParameter(void *item)
{
    return item && (TypeItem(item) == Item_ChoiceParameter);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ItemIsButton(void* item)
{
    return item && (TypeItem(item) == Item_Button);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ItemIsSButton(void* item)
{
    return item && (TypeItem(item) == Item_SButton);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Choice_NumChoices(Choice *choice)
{
    Item type = TypeItem(choice);

    int retValue = 0;

    if (type == Item_Choice)
    {
        while (choice->names[retValue * 2])
        {
            ++retValue;
        }
    }
    else if (type == Item_ChoiceParameter)
    {
        ChoiceParameter *params = (ChoiceParameter *)choice;
        for (int i = 0; i < NumParameters; i++)
        {
            if ((params->allowParameters).allow[i])
            {
                ++retValue;
            }
        }
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Item TypeItem(void *address)
{
    if (address)
    {
        return ((Choice*)address)->typeItem;
    }
    return Item_None;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int PositionOnPage(void *item)
{
    for (int numPage = 0; numPage < NUM_PAGES; numPage++)
    {
        for (int numItem = 0; numItem < NUM_ITEMS_ON_PAGE; numItem++)
        {
            if (item == menu[numPage]->items[numItem])
            {
                return numItem;
            }
        }
    }
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control ButtonForItem(void *item)
{
    int pos = PositionOnPage(item);

    if (pos >= 0)
    {
        return (Control)((int)B_F1 + pos);
    }

    return Control_None;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Choice_CurrentChoice(Choice *choice)
{
    Item type = TypeItem(choice);

    int retValue = 0;

    if (type == Item_Choice)
    {
        retValue = *(choice->cell);
    }
    else if (type == Item_ChoiceParameter)
    {
        ChoiceParameter *param = (ChoiceParameter *)choice;

        for (int i = 0; i < NumParameters; i++)
        {
            if ((param->allowParameters).allow[i] == false)
            {
                continue;
            }
            if (i == *param->numParameter)
            {
                break;
            }
            retValue++;
        }
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char* Choice_Name(Choice* choice, int num)
{
    Item type = TypeItem(choice);

    const char* retValue = 0;

    if (type == Item_Choice)
    {
        retValue = choice->names[num * 2 + LANGUAGE];
    }
    else if (type == Item_ChoiceParameter)
    {
        ChoiceParameter *param = (ChoiceParameter*)choice;

        for (int i = 0; i < NumParameters; i++)
        {
            if ((param->allowParameters).allow[i] == false)
            {
                continue;
            }
            if (num == 0)
            {
                retValue = (char*)nameParameter[i][LANGUAGE];
            }
            --num;
        }
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Page* CurrentPage(void)
{
    return (ADDITION_PAGE_IS_NONE ? (Page*)menu[CURRENT_PAGE] : ADDITION_PAGE);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char* NameParameter(WaveParameter parameter)
{
    return (char*)nameParameter[parameter][LANGUAGE];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char* NameWaveForm(WaveForm form)
{
    return (char*)nameWaveForm[form][LANGUAGE];
}
