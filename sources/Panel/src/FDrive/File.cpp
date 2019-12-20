#include "defines.h"
#include "log.h"
#include "File.h"
#include "Command.h"
#include "Display/Painter.h"
#include "Generator/Signals.h"
#include "Interface/Interface_p.h"
#include "Settings/Settings.h"
#include "Utils/String.h"
#include <cstring>


static struct Picture
{
    uint8 data[300];
    void Clear()
    {
        std::memset(data, 0, 300);
    }
} picture;



/// Порядковый номер файла в текущей папке
static int num = -1;


File::File()
{
    picture.Clear();
}


File::~File()
{
    Close();
}


void File::Draw(int x, int y)
{
    Painter::SetColor(Color::GRAY_10);

    float scale = 100.0F / 255.0F;

    Painter::SetColor(Color::GRAY_25);

    for (int i = 0; i < 240; i++)
    {
        Painter::SetPoint(x + i, (int)(y - Form::GetFormFlash(CURRENT_CHANNEL)[i] * scale));
    }

    if (num == -1)
    {
        return;
    }

    Painter::SetColor(Color::FILL);

    for (int i = 0; i < 240; i++)
    {
        Painter::SetPoint(x + i, static_cast<int>(y - picture.data[i] * scale));
    }
}


bool File::Handler(SimpleMessage *msg)
{
    uint8 com = msg->TakeByte();

    if (com == Command::FDrive_GetPictureDDS)
    {
        num = msg->TakeByte();

        msg->TakeRemainigData(picture.data);

        return true;
    }

    return false;
}


void File::SetDataToWave()
{
    Form::SetFormFlash(CURRENT_CHANNEL, picture.data);
}


static bool EqualsRequestPicture(Task *request, Task *answer)
{
    SimpleMessage *send = request->GetMessage();
    SimpleMessage *recv = answer->GetMessage();

    uint8 com = Command::FDrive_GetPictureDDS;

    return  (com == send->TakeByte()) &&
            (com == recv->TakeByte());
}


void File::Open(int n)
{
    Close();

    Message::FDrive::PictureDDS message(static_cast<uint8>(n));

    Task *task = new Task(&message, File::Handler, EqualsRequestPicture);

    Interface::AddTask(task);
}


void File::Close()
{
    num = -1;

    picture.Clear();
}
