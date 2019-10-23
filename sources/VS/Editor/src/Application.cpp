#pragma warning(push)
#pragma warning(disable:4018 4189 4355 4365 4459 4548 4571 4625 4668 4774 5026 5027)
#include "Application.h"
#include <SDL.h>
#pragma warning(pop)

#undef main


extern void update();
extern void init();


enum
{
    Minimal_Quit = wxID_EXIT,
    Minimal_Resize = wxID_HIGHEST + 1
};

enum
{
    TIMER_ID = 1,
    RESIZE_ID = 2
};


wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_MENU(Minimal_Quit, Frame::OnQuit)
    EVT_TIMER(TIMER_ID, Frame::OnTimer)
wxEND_EVENT_TABLE()


wxIMPLEMENT_APP_NO_MAIN(Application);


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    if (SDL_Init(SDL_INIT_EVENTS) != 0U) //-V2517
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

    return true;
}


Frame::Frame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title),
    timer(this, TIMER_ID)
{
    SetIcon(wxICON(sample));

    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");

    SetMenuBar(menuBar);

    CreateStatusBar(2);

    timer.Start(0);
}


void Frame::OnTimer(wxTimerEvent &)
{
    update();
    HandlerEvents();
}


void Frame::HandlerEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        //SDL_PumpEvents();
        switch (event.type)
        {
        case SDL_WINDOWEVENT:
        {
            int i = 0;
        }
            break;
        default:
            break;
        }
    }
}


void Frame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close(true);
}
