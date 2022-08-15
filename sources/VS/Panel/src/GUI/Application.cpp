// 2022/8/1 11:34:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#undef CRC
#include "Application.h"
#include "ConsoleSCPI.h"
#include "Display/Display.h"

#undef main



extern void update();
extern void init();



enum
{
    FILE_QUIT = wxID_HIGHEST + 1,

    TOOL_SCPI
};

enum
{
    TIMER_ID = 1
};


wxIMPLEMENT_APP_NO_MAIN(Application);


int main(int argc, char **argv)
{
    HWND myConsole = GetConsoleWindow();

    ShowWindow(myConsole, 0);

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
    wxMenu *toolsMenu = new wxMenu;

    fileMenu->Append(FILE_QUIT, "E&xit\tAlt-X", "Quit this program");

    toolsMenu->Append(TOOL_SCPI, "SCPI");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "Файл");
    menuBar->Append(toolsMenu, "Инструменты");

    wxFrameBase::SetMenuBar(menuBar);

    wxFrameBase::CreateStatusBar(2);

    Bind(wxEVT_MENU, &Frame::OnQuit, this, FILE_QUIT);
    Bind(wxEVT_TIMER, &Frame::OnTimer, this, TIMER_ID);
    Bind(wxEVT_MENU, &Frame::OnSCPI, this, TOOL_SCPI);
    Bind(wxEVT_CLOSE_WINDOW, &Frame::OnClose, this);

    ConsoleSCPI::Self()->Show();

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
        float fps = (float)count / (float)(TIME_MS - prevTime) * 1000.0F;

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


void Frame::OnClose(wxCloseEvent &event)
{
    ConsoleSCPI::Self()->Destroy();

    Display::Warnings::Clear();

    event.Skip();
}


void Frame::OnSCPI(wxCommandEvent &)
{
    ConsoleSCPI::Self()->SwitchVisibility();
}
