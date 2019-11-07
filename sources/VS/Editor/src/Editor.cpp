#include "Editor.h"
#pragma warning(push, 0)
#include <SDL.h>
#include <wx/wx.h>
#include <wx/mstream.h>
#pragma warning(pop)

#undef main


#include "defines.h"
#include "Canvas.h"
#include "Form.h"
#include "History.h"
#include "Dialogs/ExponentDialog.h"
#include "Dialogs/InsertPointsDialog.h"
#include "Dialogs/TrapezeDialog.h"
#include "Dialogs/TriangleDialog.h"

extern void update();
extern void init();


enum
{
    MENU_FILE_QUIT = wxID_EXIT,

    CONTEXT_MENU_DELETE = wxID_HIGHEST + 1,     // Удалить точку
    CONTEXT_MENU_PARAMETERS,                    // Параметры точки

    ALIGN_LEFT,                                 // Выровнять точку по левой
    ALIGN_RIGHT,                                // Выровнять точку по правой
    ALIGN_LEFT_TOP,                             // Выровнять точку по левой верхней
    ALIGN_LEFT_DOWN,                            // Выровнять точку по левой нижней
    ALIGN_RIGHT_TOP,                            // Выровнять точку по правой верхней
    ALIGN_RIGHT_DOWN,                           // Выровнять точку по правой нижней

    FILE_OPEN,
    FILE_SAVE,
    FILE_NEW,                                   // Очистить форму
	UNDO,
	REDO,

    CREATE_SINE,
    CREATE_TRIANGLE,
    CREATE_TRAPEZE,
    CREATE_EXPONENT,
    INSERT_POINTS
};

enum
{
    TIMER_ID = 1
};


wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_MENU(MENU_FILE_QUIT, Frame::OnQuit)
    EVT_MENU(CONTEXT_MENU_DELETE, Frame::OnDeletePoint)
    EVT_MENU(CONTEXT_MENU_PARAMETERS, Frame::OnParametersPoint)
    EVT_MENU(ALIGN_LEFT, Frame::OnAlignLeft)
    EVT_MENU(ALIGN_RIGHT, Frame::OnAlignRight)
    EVT_MENU(ALIGN_LEFT_TOP, Frame::OnAlignLeftTop)
    EVT_MENU(ALIGN_LEFT_DOWN, Frame::OnAlignLeftDown)
    EVT_MENU(ALIGN_RIGHT_TOP, Frame::OnAlignRightTop)
    EVT_MENU(ALIGN_RIGHT_DOWN, Frame::OnAlignRightDown)
    EVT_MENU(FILE_OPEN, Frame::OnOpenFile)
    EVT_MENU(FILE_SAVE, Frame::OnSaveFile)
    EVT_MENU(FILE_NEW, Frame::OnNewFile)
    EVT_MENU(UNDO, Frame::OnUndo)
    EVT_MENU(REDO, Frame::OnRedo)
    EVT_MENU(CREATE_SINE, Frame::CreateSine)
    EVT_MENU(CREATE_TRIANGLE, Frame::CreateTriangle)
    EVT_MENU(CREATE_TRAPEZE, Frame::CreateTrapeze)
    EVT_MENU(CREATE_EXPONENT, Frame::CreateExponent)
    EVT_MENU(INSERT_POINTS, Frame::InsertPoints)
    EVT_TIMER(TIMER_ID, Frame::OnTimer)
    EVT_SIZE(Frame::OnResize)
    EVT_PAINT(Frame::OnRepaint)
wxEND_EVENT_TABLE()


/// Курсор в виде руки
static SDL_Cursor *cursorHand = nullptr;
/// true, если ЛКМ находится в нажатом положении
static bool mouseIsDown = false;


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

    CreateMenu();

    CreateStatusBar(2);

    SetSizeAndPosition();

    Show(true);

    timer.Start(0);
}


Frame::~Frame()
{
    delete TheForm;
    delete TheCanvas;
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


void Frame::OnRepaint(wxPaintEvent &)
{
    TheCanvas->Redraw();
}


void Frame::HandlerEvents()
{
    SDL_Event event;

    static int mouseX = 0;
    static int mouseY = 0;

    while (SDL_PollEvent(&event))
    {
        SDL_PumpEvents();
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

            if (mouseIsDown)
            {
                TheForm->MovePoint(mouseX, mouseY);
            }

            TheCanvas->Redraw();

            break;

        case SDL_MOUSEBUTTONDOWN:
            mouseX = event.button.x;
            mouseY = event.button.y;
                
            if (event.button.button == 1)               // "1" соотвествует ЛКМ
            {
                if (TheForm->ExistPoint(mouseX, mouseY, false))
                {
                    mouseIsDown = true;
                }
                else
                {
                    TheForm->SetPoint(mouseX, mouseY);
                }
                
            }
            else if (event.button.button == 3)          // "3" соответствует ПКМ
            {
                mouseIsDown = false;

                ShowContextMenu({ mouseX, mouseY}, TheForm->ExistPoint(mouseX, mouseY, false));
            }
            else
            {
                // остальные кнопки не обрабатывем
            }
            break;

        case SDL_MOUSEBUTTONUP:
            mouseIsDown = false;
            History::Add(TheForm);
            break;

        default:
            // ничего не делать
            break;
        }
    }

    if (TheForm->ExistPoint(mouseX, mouseY, mouseIsDown))
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

	wxSize displaySize = wxGetDisplaySize();
	displaySize.y -= 40;

	if (displaySize.y < size.y)
	{
		size.y = displaySize.y;
	}

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


void Frame::CreateMenu()
{
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(MENU_FILE_QUIT, "E&xit\tAlt-X", "Quit this program");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");

    SetMenuBar(menuBar);

    wxBitmap imgOpen(wxImage(wxT("icons/open.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgSave(wxImage(wxT("icons/save.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgNew(wxImage(wxT("icons/new.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgUndo(wxImage(wxT("icons/undo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgRedo(wxImage(wxT("icons/redo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateSine(wxImage(wxT("icons/sine.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateTriangle(wxImage(wxT("icons/triangle.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateTrapeze(wxImage(wxT("icons/trapeze.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateExponent(wxImage(wxT("icons/exponent.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgInsertPoints(wxImage(wxT("icons/points.bmp"), wxBITMAP_TYPE_BMP));

    wxToolBar* toolBar = CreateToolBar();
    toolBar->AddTool(FILE_OPEN, wxT("Открыть"), imgOpen, wxT("Загрузить ранее созданный сигнал из файла"));
    toolBar->AddTool(FILE_SAVE, wxT("Сохранить"), imgSave, wxT("Сохранить сигнал в файла"));
    toolBar->AddTool(FILE_NEW, wxT("Новый"), imgNew, wxT("Создать новый сигнал"));

    toolBar->AddSeparator();
    toolBar->AddTool(UNDO, wxT("Отменить"), imgUndo, wxT("Отменить предыдущее действие"));
    toolBar->AddTool(REDO, wxT("Восстановить"), imgRedo, wxT("Восстановить следующее действие"));

    toolBar->AddSeparator();
    toolBar->AddTool(CREATE_SINE, wxT("Синусоида"), imgCreateSine, wxT("Создать новый сигнал в форме синусоиды"));
    toolBar->AddTool(CREATE_TRIANGLE, wxT("Треугольник"), imgCreateTriangle, wxT("Создать новый сигнал в форме треугольника"));
    toolBar->AddTool(CREATE_TRAPEZE, wxT("Трапеция"), imgCreateTrapeze, wxT("Создать новый сигнал в форме трапеции"));
    toolBar->AddTool(CREATE_EXPONENT, wxT("Експонента"), imgCreateExponent, wxT("Создать новый экспоненциальный сигнал"));

    toolBar->AddSeparator();
    toolBar->AddTool(INSERT_POINTS, wxT("Вставить точки"), imgInsertPoints, wxT("Вставить маркеры"));
    toolBar->Realize();
}


void Frame::ShowContextMenu(const wxPoint &pos, bool underPoint)
{
    static wxMenu menuPoint;
    static wxMenu menuContext;
    static wxMenu *menuAlign = nullptr;

    if (menuAlign == nullptr)
    {
        menuAlign = new wxMenu();

        menuAlign->Append(ALIGN_LEFT, "Слева");
        menuAlign->Append(ALIGN_RIGHT, "Справа");
        menuAlign->Append(ALIGN_LEFT_TOP, "Слева вверху");
        menuAlign->Append(ALIGN_LEFT_DOWN, "Слева внизу");
        menuAlign->Append(ALIGN_RIGHT_TOP, "Справа вверху");
        menuAlign->Append(ALIGN_RIGHT_DOWN, "Справа внизу");

        menuPoint.Append(CONTEXT_MENU_DELETE, "Удалить");
        menuPoint.Append(CONTEXT_MENU_PARAMETERS, "Параметры");
        menuPoint.AppendSubMenu(menuAlign, "Выровнять");

        menuContext.Append(FILE_NEW, "Очистить");
    }

    PopupMenu(underPoint ? &menuPoint : &menuContext, pos.x, pos.y);
}


void Frame::OnDeletePoint(wxCommandEvent &)
{
    TheForm->RemovePoint();
}


void Frame::OnParametersPoint(wxCommandEvent &)
{

}


void Frame::OnAlignLeft(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::Left);
}


void Frame::OnAlignRight(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::Right);
}


void Frame::OnAlignLeftTop(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::LeftTop);
}


void Frame::OnAlignLeftDown(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::LeftDown);
}


void Frame::OnAlignRightTop(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::RightTop);
}

void Frame::OnAlignRightDown(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::RightDown);
}

void Frame::OnUndo(wxCommandEvent&)
{
	TheForm->UndoHistory();
}

void Frame::OnRedo(wxCommandEvent&)
{
	TheForm->RedoHistory();
}

void Frame::OnOpenFile(wxCommandEvent &)
{
    wxFileDialog openDialog(nullptr, wxEmptyString, wxEmptyString, wxEmptyString, wxT("*.*"), wxFD_OPEN);
    if (openDialog.ShowModal() == wxID_OK)
    {
        wxString path = openDialog.GetPath();
        std::cout << path.c_str() << std::endl;
    }
}

void Frame::OnSaveFile(wxCommandEvent &)
{
    wxFileDialog saveDialog(nullptr, wxT("Сохранить"), wxEmptyString, wxEmptyString, wxT("*.*"), wxFD_SAVE);
    if (saveDialog.ShowModal() == wxID_OK)
    {
        wxString path = saveDialog.GetPath();
        std::cout << path.c_str() << std::endl;
    }
}

void Frame::OnNewFile(wxCommandEvent &)
{
    TheForm->Clear();

    History::Add(TheForm);
}

void Frame::CreateSine(wxCommandEvent &)
{
    static uint16 data[Point::NUM_POINTS];

    for (int i = 0; i < Point::NUM_POINTS; i++)
    {
        data[i] = static_cast<uint16>(Point::AVE_VALUE + (std::sin(static_cast<float>(i) / Point::NUM_POINTS * 2 * 3.14) * Point::AVE_VALUE));
    }

    TheForm->SetMainForm(data, nullptr);
}

void Frame::CreateTriangle(wxCommandEvent &)
{
    TriangleDialog dialog;

    dialog.ShowModal();
}

void Frame::CreateTrapeze(wxCommandEvent &)
{
    TrapezeDialog dialog;

    dialog.ShowModal();
}

void Frame::CreateExponent(wxCommandEvent &)
{
    ExponentDialog dialog;

    dialog.ShowModal();
}

void Frame::InsertPoints(wxCommandEvent &)
{
    InsertPointsDialog dialog;

    dialog.ShowModal();
}
