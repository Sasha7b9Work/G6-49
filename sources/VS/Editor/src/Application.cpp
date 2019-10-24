#include "Application.h"
#pragma warning(push, 0)
#include <SDL.h>
#include <wx/display.h>
#pragma warning(pop)

#undef main


#include "defines.h"
#include "Canvas.h"


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
    fileMenu->Append(MENU_FILE_QUIT, "E&xit\tAlt-X", "Quit this program");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");

    SetMenuBar(menuBar);

    CreateStatusBar(2);

    SetSizeAndPosition();

    TheCanvas = new Canvas(this, 640, 480);

    timer.Start(0);
}


void Frame::OnTimer(wxTimerEvent &)
{
    update();
    HandlerEvents();
}


void Frame::OnResize(wxSizeEvent &event)
{
    TheCanvas->Resize(this->GetClientSize());
}


void Frame::HandlerEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        SDL_PumpEvents();
        //switch (event.type)
        //{
        //    break;
        //default:
        //    // ничего не делать
        //    break;
        //}
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
