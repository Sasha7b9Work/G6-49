#include "MenuItems.h"
#include "Menu.h"
#include "Hardware/Controls.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static const char *nameWaveForm[NumForms][2] =
{
    {"ÑÈÍÓÑ",    "SINE"},
    {"ÏÈËÀ",     "SAW"},
    {"ÈÌÏÓËÜÑ",  "IMPULSE"}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice_CurrentName(Choice* choice)
{
    return choice->names[(*(choice->cell)) * 2 + LANGUAGE];
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
char* NameParameter(WaveParameter parameter)
{
    return (char*)nameParameter[parameter][LANGUAGE];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char* NameWaveForm(WaveForm form)
{
    return (char*)nameWaveForm[form][LANGUAGE];
}
