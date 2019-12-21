#include "Application.h"

#undef main



extern void update();
extern void init();



enum
{
    Minimal_Quit = wxID_EXIT
};

enum
{
    TIMER_ID = 1
};

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_MENU(Minimal_Quit, Frame::OnQuit)
    EVT_TIMER(TIMER_ID, Frame::OnTimer)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP_NO_MAIN(Application);



int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

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



Frame::Frame(const wxString& title)
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
    SetStatusText("Welcome to wxWidgets!");

    timer.Start(0);
}


void Frame::OnTimer(wxTimerEvent&)
{
    update();

    HandlerEvents();

    DrawFPS();
}


void Frame::HandlerEvents()
{
}


void Frame::DrawFPS()
{
    static int count = 0;
    static unsigned int prevTime = 0;

    count++;

    if (TIME_MS - prevTime > 1000)
    {
        float fps = static_cast<float>(count) / (TIME_MS - prevTime) * 1000.0F;

        char buffer[100];
        sprintf(buffer, "fps %f", fps);

        SetStatusText(buffer);

        prevTime = TIME_MS;
        count = 0;
    }
}


void Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}
