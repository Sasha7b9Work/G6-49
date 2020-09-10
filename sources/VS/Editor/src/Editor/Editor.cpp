#include "Editor/Editor.h"
#include "Editor/Settings.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/display.h>
#include <wx/mstream.h>
#include <wx/textfile.h>
#pragma warning(pop)

#undef main


#include "defines.h"
#include "Form.h"
#include "Dialogs/ExponentDialog.h"
#include "Dialogs/GaussDialog.h"
#include "Dialogs/InsertPointsDialog.h"
#include "Dialogs/ParametersDialog.h"
#include "Dialogs/SinXDialog.h"
#include "Dialogs/TrapezeDialog.h"
#include "Dialogs/TriangleDialog.h"
#include "Editor/History.h"
#include "Editor/Selector.h"
#include "Editor/Painter/Canvas.h"
#include "Editor/Painter/Zoomer.h"
#include "Windows/CompositeSignalWindow.h"


extern void update();
extern void init();


Frame *TheFrame = nullptr;

static uint16 indexPoint = 0;
static uint16 valuePoint = 0;


enum //-V2521
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

    CREATE_TRIANGLE,
    CREATE_TRAPEZE,
    CREATE_EXPONENT,
    CREATE_SINX,
    CREATE_GAUSS,
    CREATE_GAVERSINE,
    CREATE_NOISE,
    CREATE_COMPISITE,

    MBL_POINTS,
    MBL_LINES,

    MODE_SELECT,

    SCALE_MORE,
    SCALE_LESS,
    SCALE_REGION,

    CHANGE_TYPE_GRID,

    INSERT_POINTS
};

enum
{
    TIMER_ID = 1
};


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


Frame::Frame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title),
    timer(this, TIMER_ID)
{
    SetIcon(wxICON(sample));

    CreateMenu();

    CreateStatusBar(2);

    SetSizeAndPosition();

    Bind(wxEVT_MENU,     &Frame::OnQuit,            this, MENU_FILE_QUIT);
    Bind(wxEVT_MENU,     &Frame::OnDeleteMarker,    this, CONTEXT_MENU_DELETE);
    Bind(wxEVT_MENU,     &Frame::OnParametersPoint, this, CONTEXT_MENU_PARAMETERS);
    Bind(wxEVT_MENU,     &Frame::OnAlignLeft,       this, ALIGN_LEFT);
    Bind(wxEVT_MENU,     &Frame::OnAlignRight,      this, ALIGN_RIGHT);
    Bind(wxEVT_MENU,     &Frame::OnAlignLeftTop,    this, ALIGN_LEFT_TOP);
    Bind(wxEVT_MENU,     &Frame::OnAlignLeftDown,   this, ALIGN_LEFT_DOWN);
    Bind(wxEVT_MENU,     &Frame::OnAlignRightTop,   this, ALIGN_RIGHT_TOP);
    Bind(wxEVT_MENU,     &Frame::OnAlignRightDown,  this, ALIGN_RIGHT_DOWN);
    Bind(wxEVT_MENU,     &Frame::OnOpenFile,        this, FILE_OPEN);
    Bind(wxEVT_MENU,     &Frame::OnSaveFile,        this, FILE_SAVE);
    Bind(wxEVT_MENU,     &Frame::OnNewFile,         this, FILE_NEW);
    Bind(wxEVT_MENU,     &Frame::OnUndo,            this, UNDO);
    Bind(wxEVT_MENU,     &Frame::OnRedo,            this, REDO);
    Bind(wxEVT_MENU,     &Frame::CreateTriangle,    this, CREATE_TRIANGLE);
    Bind(wxEVT_MENU,     &Frame::CreateTrapeze,     this, CREATE_TRAPEZE);
    Bind(wxEVT_MENU,     &Frame::CreateExponent,    this, CREATE_EXPONENT);
    Bind(wxEVT_MENU,     &Frame::CreateSinX,        this, CREATE_SINX);
    Bind(wxEVT_MENU,     &Frame::CreateGauss,       this, CREATE_GAUSS);
    Bind(wxEVT_MENU,     &Frame::CreateGaversine,   this, CREATE_GAVERSINE);
    Bind(wxEVT_MENU,     &Frame::CreateNoise,       this, CREATE_NOISE);
    Bind(wxEVT_MENU,     &Frame::CreateComposite,   this, CREATE_COMPISITE);
    Bind(wxEVT_MENU,     &Frame::SetPointsMBL,      this, MBL_POINTS);
    Bind(wxEVT_MENU,     &Frame::SetLinesMBL,       this, MBL_LINES);
    Bind(wxEVT_MENU,     &Frame::SetModeSelect,     this, MODE_SELECT);
    Bind(wxEVT_MENU,     &Frame::OnScaleMore,       this, SCALE_MORE);
    Bind(wxEVT_MENU,     &Frame::OnScaleLess,       this, SCALE_LESS);
    Bind(wxEVT_MENU,     &Frame::OnScaleRegion,     this, SCALE_REGION);
    Bind(wxEVT_MENU,     &Frame::OnChangeTypeGrid,  this, CHANGE_TYPE_GRID);
    Bind(wxEVT_MENU,     &Frame::InsertPoints,      this, INSERT_POINTS);
    Bind(wxEVT_TIMER,    &Frame::OnTimer,           this, TIMER_ID);
    Bind(wxEVT_PAINT,    &Frame::OnRepaint,         this);
    Bind(wxEVT_KEY_DOWN, &Frame::OnKeyDown,         this);

    Show(true);

    timer.Start(0);

    TheFrame = this;

    ModeButtonLeft::Set(ModeButtonLeft::EditLines);

    GetStatusBar()->SetFieldsCount(3);

    int widths[3] = { -1, 100, 95 };

    GetStatusBar()->SetStatusWidths(3, widths);

    UpdateStatusBar();
}


Frame::~Frame()
{
    delete TheForm;
    delete TheCanvas;
}


void Frame::OnTimer(wxTimerEvent &)
{
    update();
}


void Frame::OnRepaint(wxPaintEvent &)
{
    TheCanvas->Redraw();
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


void Frame::CreateMenu()
{
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(FILE_OPEN, wxT("Загрузить\tCtrl+O"), wxT("Загрузить данные из файла"));
    fileMenu->Append(FILE_SAVE, wxT("Сохранить\tCtrl+S"), wxT("Сохранить данные в файл"));
    fileMenu->AppendSeparator();
    fileMenu->Append(MENU_FILE_QUIT, wxT("Выход\tAlt+X"), wxT("Закрыть редактор"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("Файл"));

    SetMenuBar(menuBar);

    toolBar = CreateToolBar();
    AddTool(FILE_OPEN, wxT("Загрузить ранее созданный сигнал из файла"), "open.bmp");
    AddTool(FILE_SAVE, wxT("Сохранить сигнал в файл"), "save.bmp");
    AddTool(FILE_NEW, wxT("Создать новый сигнал"), "new.bmp");

    toolBar->AddSeparator();
    AddTool(UNDO, wxT("Отменить предыдущее действие"), "undo.bmp");
    AddTool(REDO, wxT("Восстановить следующее действие"), "redo.bmp");

    toolBar->AddSeparator();
    AddTool(CREATE_TRIANGLE, wxT("Создать новый сигнал в форме треугольника"), "triangle.bmp");
    AddTool(CREATE_TRAPEZE, wxT("Создать новый сигнал в форме трапеции"), "trapeze.bmp");
    AddTool(CREATE_EXPONENT, wxT("Создать новый экспоненциальный сигнал"), "exponent.bmp");
    AddTool(CREATE_SINX, wxT("Создать сигнал вида sin(x)/x"), "sinx.bmp");
    AddTool(CREATE_GAUSS, wxT("Создать новый сигнал в виде гуссовой функции"), "gauss.bmp");
    AddTool(CREATE_GAVERSINE, wxT("Создать новый сигнал в форме гаверсинуса"), "gaversine.bmp");
    AddTool(CREATE_NOISE, wxT("Создать шумовой сигнал"), "noise.bmp");
    AddTool(CREATE_COMPISITE, wxT("Создать сложный сигнал из гармоник"), "composite.bmp");

    toolBar->AddSeparator();
    AddTool(INSERT_POINTS, wxT("Вставить маркеры"), "points.bmp", "points_disable.bmp");

    toolBar->AddSeparator();
    AddRadioTool(MBL_POINTS, wxT("Режим редактирования точек"), "MBL_points.bmp");
    AddRadioTool(MBL_LINES, wxT("Режим редактирования интерполирующими прямыми"), "MBL_lines.bmp");

    toolBar->AddSeparator();
    AddCheckTool(MODE_SELECT, wxT("Режим выделения"), "MBL_select.bmp");

    toolBar->AddSeparator();
    AddTool(SCALE_MORE, wxT("Увеличить масштаб"), "scale_more.bmp");
    AddTool(SCALE_LESS, wxT("Уменьшить масштаб"), "scale_less.bmp");
    AddTool(SCALE_REGION, wxT("Показать выделенную область"), "scale_region.bmp");

    toolBar->AddSeparator();
    AddTool(CHANGE_TYPE_GRID, wxT("Изменение размерности линий сетки - проценты"), "grid_percents.bmp");

    toolBar->Realize();
}


void Frame::AddTool(int id, const wxString &label, const char *file, const char *fileDisabld)
{
    wxBitmap bitmap(wxImage(wxString("icons/") + wxString(file), wxBITMAP_TYPE_BMP));

    wxBitmap bitmapDisabled = (fileDisabld == nullptr) ? bitmap : wxBitmap(wxImage(wxString("icons/") + wxString(fileDisabld), wxBITMAP_TYPE_BMP));

    toolBar->AddTool(id, label, bitmap, bitmapDisabled, wxITEM_NORMAL, label, label);
}


void Frame::AddRadioTool(int id, const wxString &label, const char *file)
{
    wxBitmap bitmap(wxImage(wxString("icons/") + wxString(file), wxBITMAP_TYPE_BMP));

    toolBar->AddRadioTool(id, label, bitmap, bitmap, label, label);
}


void Frame::AddCheckTool(int id, const wxString &label, const char *file)
{
    wxBitmap bitmap(wxImage(wxString("icons/") + wxString(file), wxBITMAP_TYPE_BMP));

    toolBar->AddCheckTool(id, label, bitmap, bitmap, label, label);
}


void Frame::ShowContextMenu(const wxPoint &pos)
{
    bool underPoint = TheForm->ExistMarker(pos.x, pos.y, false, &indexPoint, &valuePoint);

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

        menuPoint.Append(CONTEXT_MENU_PARAMETERS, "Изменить");
        menuPoint.AppendSubMenu(menuAlign, "Выровнять");
        menuPoint.Append(CONTEXT_MENU_DELETE, "Удалить");

        menuContext.Append(FILE_NEW, "Очистить");
    }

    PopupMenu(underPoint ? &menuPoint : &menuContext, pos.x, pos.y);
}


void Frame::OnParametersPoint(wxCommandEvent &)
{
    ParametersDialog(indexPoint, valuePoint).ShowModal();
}


void Frame::OnDeleteMarker(wxCommandEvent &)
{
    TheForm->RemoveCurrentMarker();
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
    wxFileDialog openDialog(nullptr, wxEmptyString, wxEmptyString, wxEmptyString, wxT("*.sig"), wxFD_OPEN);

    if (openDialog.ShowModal() == wxID_OK)
    {
        wxString fileName = openDialog.GetPath();

        wxTextFile file(fileName);

        if(file.Exists())
        {
            file.Open();

            TheForm->LoadFromFile(file);

            file.Close();
        }
    }
}

void Frame::OnSaveFile(wxCommandEvent &)
{
    wxFileDialog saveDialog(nullptr, wxT("Сохранить"), wxEmptyString, wxEmptyString, wxT("*.sig"), wxFD_SAVE);

    if (saveDialog.ShowModal() == wxID_OK)
    {
        wxString fileName = saveDialog.GetPath();
        
        wxTextFile file(fileName);

        if(file.Exists())
        {
            wxMessageDialog message(this, wxT("Файл с таким именем уже существует. Перезаписать?"), wxMessageBoxCaptionStr, wxOK | wxCANCEL | wxCENTRE);
            if(message.ShowModal() == wxID_CANCEL)
            {
                return;
            }
        }
        
        file.Create();

        TheForm->SaveToFile(file);

        file.Write();

        file.Close();
    }
}

void Frame::OnNewFile(wxCommandEvent &)
{
    TheFrame->SetBlockingCanvas(false);

    TheForm->Clear();

    History::Add(TheForm);
}

void Frame::CreateTriangle(wxCommandEvent &)
{
    TriangleDialog().ShowModal();
}


void Frame::CreateTrapeze(wxCommandEvent &)
{
    TrapezeDialog().ShowModal();
}


void Frame::CreateExponent(wxCommandEvent &)
{
    ExponentDialog().ShowModal();
}


void Frame::CreateSinX(wxCommandEvent &)
{
    SinXDialog().ShowModal();
}


void Frame::CreateGauss(wxCommandEvent &)
{
    GaussDialog().ShowModal();
}


void Frame::CreateGaversine(wxCommandEvent &)
{
    static uint16 data[Point::AMOUNT];

    for (int i = 0; i < Point::AMOUNT; i++)
    {
        float sinx = std::sinf(static_cast<float>(i) / Point::AMOUNT * 2.0F * 3.14F);

        data[i] = static_cast<uint16>(Point::AVE + static_cast<uint16>(sinx * sinx * Point::AVE));
    }

    TheFrame->SetBlockingCanvas(true);

    TheForm->SetMainForm(data, nullptr);
}


void Frame::CreateNoise(wxCommandEvent &)
{
    static uint16 data[Point::AMOUNT];

    for (int i = 0; i < Point::AMOUNT; i++)
    {
        float x = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

        data[i] = static_cast<uint16>(x * Point::MAX + 0.5F);
    }

    TheFrame->SetBlockingCanvas(true);

    TheForm->SetMainForm(data, nullptr);
}


void Frame::CreateComposite(wxCommandEvent &)
{
    CompositeSignalWindow window;
    window.Create(this, wxID_ANY);
    window.Show(true);
    window.Enable(true);
}


void Frame::InsertPoints(wxCommandEvent &)
{
    if (!TheFrame->IsBlockingCanvas())
    {
        InsertPointsDialog().ShowModal();
    }
}


void Frame::SetBlockingCanvas(bool blocking)
{
    isBlockingCanvas = blocking;

    toolBar->EnableTool(INSERT_POINTS, !isBlockingCanvas);
}


void Frame::SetPointsMBL(wxCommandEvent &)
{
    ModeButtonLeft::Set(ModeButtonLeft::EditPoints);
}


void Frame::SetLinesMBL(wxCommandEvent &)
{
    ModeButtonLeft::Set(ModeButtonLeft::EditLines);
}


void Frame::SetModeSelect(wxCommandEvent &)
{
    toolBar->GetToolState(MODE_SELECT) ? Selector::Enable() : Selector::Disable();
}


void Frame::SetModeMBL()
{
    switch (ModeButtonLeft::Get())
    {
    case ModeButtonLeft::EditPoints:   toolBar->ToggleTool(MBL_POINTS, true);   break;
    case ModeButtonLeft::EditLines:    toolBar->ToggleTool(MBL_LINES, true);    break;
    }
}


void Frame::OnKeyDown(wxKeyEvent &)
{

}


void Frame::OnScaleMore(wxCommandEvent &)
{
    Zoomer::Increase();
}


void Frame::OnScaleLess(wxCommandEvent &)
{
    Zoomer::Decrease();
}


void Frame::OnScaleRegion(wxCommandEvent &)
{
    Zoomer::SetOnRegion();
}


void Frame::OnChangeTypeGrid(wxCommandEvent &)
{
    Grid::ChangeTypeGrid();

    if (Grid::TypeIsPercents())
    {
        toolBar->SetToolNormalBitmap(CHANGE_TYPE_GRID, wxBitmap(wxT("icons/grid_percents.bmp"), wxBITMAP_TYPE_BMP));
        toolBar->SetToolLongHelp(CHANGE_TYPE_GRID, wxT("Изменение размерности линий сетки - проценты"));
        toolBar->SetToolShortHelp(CHANGE_TYPE_GRID, wxT("Изменение размерности линий сетки - проценты"));
    }
    else
    {
        toolBar->SetToolNormalBitmap(CHANGE_TYPE_GRID, wxBitmap(wxT("icons/grid_points.bmp"), wxBITMAP_TYPE_BMP));
        toolBar->SetToolLongHelp(CHANGE_TYPE_GRID, wxT("Изменение размерности линий сетки - точки"));
        toolBar->SetToolShortHelp(CHANGE_TYPE_GRID, wxT("Изменение размерности линий сетки - точки"));
    }

    UpdateStatusBar();
}


void Frame::UpdateStatusBar()
{
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Сетка: %s"), Grid::GetScale()), 1);

    GetStatusBar()->PushStatusText(wxString::Format(wxT("Масштаб:  %d%%"), Zoomer::Scale()), 2);

    GetStatusBar()->Refresh();
}


void Frame::OnMouseMove(wxMouseEvent &event) //-V2009
{
    if (TheCanvas->mouseIsDown)
    {
        TheCanvas->mouseIsDown = TheCanvas->mouseInWindow;
    }

    event.GetPosition(&TheCanvas->mouseX, &TheCanvas->mouseY);

    if (mouseIsDown)
    {
        if (Zoomer::MoveWindow(mouseX))
        {
            SetMouseCursor();
            return;
        }
    }

    if (Zoomer::UnderMouse(mouseX, mouseY))
    {
        SetMouseCursor();
    }
    else if (Selector::IsEnabled())
    {
        if (mouseIsDown)
        {
            Selector::MoveBorder(mouseX);
            Redraw();
        }
        SetMouseCursor();
    }
    else if (ModeButtonLeft::Get() == ModeButtonLeft::EditLines)
    {
        if (mouseIsDown)
        {
            TheForm->MoveMarker(mouseX, mouseY);
        }

        SetMouseCursor();

        Redraw();
    }
    else if (ModeButtonLeft::Get() == ModeButtonLeft::EditPoints)
    {

    }
}
