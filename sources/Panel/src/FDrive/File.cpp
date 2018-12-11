#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "File.h"
#include "Utils/String.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������ ��� �����
static String name;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������� ������ �� ������ i-� ������ � �����
extern void SendRequestForString(int i);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void File::Draw(int /*x*/, int /*y*/)
{
    if (name.IsEmpty())
    {
        return;
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void File::Open(char *fullName)
{
    Close();
    name.From(fullName);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String File::FullName()
{
    return String(name.CString());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void File::Close()
{
    name.Release();
}