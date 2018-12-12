#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "File.h"
#include "Command.h"
#include "Display/Painter.h"
#include "Generator/Signals.h"
#include "Interface/InterfacePanel.h"
#include "Settings/Settings.h"
#include "Utils/String.h"
#include <cstring>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct Picture
{
    uint8 data[300];
    void Clear()
    {
        std::memset(data, 300, 0);
    }
} picture;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Порядковый номер файла в текущей папке
static int num = -1;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
File::File()
{
    picture.Clear();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
File::~File()
{
    Close();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void File::Draw(int x, int y)
{
    if (num == -1)
    {
        return;
    }

    Painter::SetColor(Color::GRAY_10);
    
    WAVE(CURRENT_CHANNEL).GetForm(Form::DDS)->DrawUGO(CURRENT_CHANNEL, 120);
    
    float scale = 100.0f / 255.0f;

    Painter::SetColor(Color::FILL);

    for (int i = 0; i < 240; i++)
    {
        Painter::SetPoint(x + i, (int)(y - picture.data[i] * scale));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool File::Handler(Message *msg)
{
    msg->TakeByte();
    num = msg->TakeByte();

    for (int i = 0; i < 300; i++)
    {
        picture.data[i] = msg->Data(2)[i];
    }

    Form::SetFormFlash(CURRENT_CHANNEL, msg->Data(2));

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool EqualsRequestPicture(Task *request, Task *answer)
{
    Message *send = request->GetMessage();
    Message *recv = answer->GetMessage();

    uint8 com = Command::FDrive_GetPictureDDS;

    return  (com == send->TakeByte()) &&
            (com == recv->TakeByte());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void File::Open(int _num)
{
    Close();

    Message *message = new Message(2, Command::FDrive_GetPictureDDS, (uint8)_num);

    Task *task = new Task(message, File::Handler, EqualsRequestPicture);

    Interface::AddTask(task);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void File::Close()
{
    num = -1;

    picture.Clear();
}
