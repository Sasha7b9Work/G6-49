#include "Editor.h"
#pragma warning(push, 0)
#include <SDL.h>
#include <wx/display.h>
#pragma warning(pop)

#undef main


#include "defines.h"
#include "Canvas.h"
#include "Form.h"


extern void update();
extern void init();


enum
{
    MENU_FILE_QUIT = wxID_EXIT
};

enum
{
    TIMER_ID = 1
};


wxBEGIN_EVENT_TABLE(Frame, wxFrame)
EVT_MENU(MENU_FILE_QUIT, Frame::OnQuit)
EVT_TIMER(TIMER_ID, Frame::OnTimer)
EVT_SIZE(Frame::OnResize)
wxEND_EVENT_TABLE()


SDL_Cursor *cursorHand = nullptr;


wxIMPLEMENT_APP_NO_MAIN(Application);


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0U) //-V2517
    {
        std::cout << "SDL_Init Error:" << SDL_GetError() << std::endl;
    }

    return wxEntry(argc, argv);
}


bool Application::OnInit()
{
    if (!wxApp::OnInit())
    {
        return false;
    }

    init();

    cursorHand = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    return true;
}


Frame::Frame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title),
    timer(this, TIMER_ID)
{
    SetIcon(wxICON(sample));

    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(MENU_FILE_QUIT, "E&xit\tAlt-X", "Quit this program");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");

    SetMenuBar(menuBar);

    CreateStatusBar(2);

    SetSizeAndPosition();

    Show(true);

    timer.Start(0);
}


void Frame::OnTimer(wxTimerEvent &)
{
    HandlerEvents();
    update();
    DrawFPS();
}


void Frame::OnResize(wxSizeEvent &)
{
    TheCanvas->Resize(this->GetClientSize());
}


void Frame::HandlerEvents()
{
    SDL_Event event;

    static int mouseX = 0;
    static int mouseY = 0;

    while (SDL_PollEvent(&event))
    {
        //SDL_PumpEvents();
        switch (event.type)
        {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                Close(true);
            }
            break;

        case SDL_MOUSEMOTION:
            mouseX = event.motion.x;
            mouseY = event.motion.y;

            break;

        case SDL_MOUSEBUTTONDOWN:
            mouseX = event.button.x;
            mouseY = event.button.y;
                
            if (event.button.button == 1)               // "1" соотвествует ЛКМ
            {
                TheForm->SetPoint(mouseX, mouseY);
            }
            else if (event.button.button == 3)          // "3" соответствует ПКМ
            {
                if (TheForm->ExistPoint(mouseX, mouseY))
                {
                    ShowContextMenu({ mouseX, mouseY });
                }
            }
            break;

        default:
            // ничего не делать
            break;
        }
    }

    if (TheForm->ExistPoint(mouseX, mouseY))
    {
        SDL_SetCursor(cursorHand);
    }
}


void Frame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close(true);
}


void Frame::SetSizeAndPosition()
{
    wxSize size = { 1024, 768 };

    SetSize(size);
    SetMinSize({ 640, 480 });

    wxRect rect = GetMaxDisplay();

    SetPosition({ rect.width / 2 - size.GetWidth() / 2, rect.height / 2 - size.GetHeight() / 2 });
}


wxRect Frame::GetMaxDisplay()
{
    wxRect result = { 0, 0, 0, 0 };

    for (uint i = 0; i < wxDisplay::GetCount(); i++)
    {
        wxDisplay display(i);

        wxRect rect = display.GetClientArea();
        if (rect.width > result.width)
        {
            result.width = rect.width;
            result.height = rect.height;
        }
    }

    return result;
}


void Frame::DrawFPS()
{
    static int count = 0;
    static unsigned int prevTime = 0;

    count++;

    if (SDL_GetTicks() - prevTime > 1000)
    {
        float fps = static_cast<float>(count) / (SDL_GetTicks() - prevTime) * 1000.0F;

        char buffer[100];
        sprintf(buffer, "fps %f", fps);

        SetStatusText(buffer);

        prevTime = SDL_GetTicks();
        count = 0;
    }
}


void Frame::ShowContextMenu(const wxPoint &pos)
{
    wxMenu menu;
    menu.AppendSeparator();
    PopupMenu(&menu, pos.x, pos.y);
}
