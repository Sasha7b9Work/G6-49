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

    CONTEXT_MENU_DELETE = wxID_HIGHEST + 1,     // ������� �����
    CONTEXT_MENU_PARAMETERS,                    // ��������� �����

    ALIGN_LEFT,                                 // ��������� ����� �� �����
    ALIGN_RIGHT,                                // ��������� ����� �� ������
    ALIGN_LEFT_TOP,                             // ��������� ����� �� ����� �������
    ALIGN_LEFT_DOWN,                            // ��������� ����� �� ����� ������
    ALIGN_RIGHT_TOP,                            // ��������� ����� �� ������ �������
    ALIGN_RIGHT_DOWN,                           // ��������� ����� �� ������ ������

    FILE_OPEN,
    FILE_SAVE,
    FILE_NEW,                                   // �������� �����
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
    SetIcon(wxICON(MAIN_ICON));

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
    fileMenu->Append(FILE_OPEN, wxT("���������\tCtrl+O"), wxT("��������� ������ �� �����"));
    fileMenu->Append(FILE_SAVE, wxT("���������\tCtrl+S"), wxT("��������� ������ � ����"));
    fileMenu->AppendSeparator();
    fileMenu->Append(MENU_FILE_QUIT, wxT("�����\tAlt+X"), wxT("������� ��������"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("����"));

    SetMenuBar(menuBar);

    toolBar = CreateToolBar();

    AddTool(FILE_OPEN, wxT("��������� ����� ��������� ������ �� �����"), "TOOL_OPEN");
    AddTool(FILE_SAVE, wxT("��������� ������ � ����"), "TOOL_SAVE");
    AddTool(FILE_NEW, wxT("������� ����� ������"), "TOOL_NEW");

    toolBar->AddSeparator();
    AddTool(UNDO, wxT("�������� ���������� ��������"), "TOOL_UNDO");
    AddTool(REDO, wxT("������������ ��������� ��������"), "TOOL_REDO");

    toolBar->AddSeparator();
    AddTool(CREATE_TRIANGLE, wxT("������� ����� ������ � ����� ������������"), "TOOL_TRIANGLE");
    AddTool(CREATE_TRAPEZE, wxT("������� ����� ������ � ����� ��������"), "TOOL_TRAPEZE");
    AddTool(CREATE_EXPONENT, wxT("������� ����� ���������������� ������"), "TOOL_EXPONENT");
    AddTool(CREATE_SINX, wxT("������� ������ ���� sin(x)/x"), "TOOL_SINX");
    AddTool(CREATE_GAUSS, wxT("������� ����� ������ � ���� �������� �������"), "TOOL_GAUSS");
    AddTool(CREATE_GAVERSINE, wxT("������� ����� ������ � ����� �����������"), "TOOL_GAVERSINE");
    AddTool(CREATE_NOISE, wxT("������� ������� ������"), "TOOL_NOISE");
    AddTool(CREATE_COMPISITE, wxT("������� ������� ������ �� ��������"), "TOOL_COMPISITE");

    toolBar->AddSeparator();
    AddTool(INSERT_POINTS, wxT("�������� �������"), "TOOL_POINTS", "TOOL_POINTS_DISABLE");

    toolBar->AddSeparator();
    AddRadioTool(MBL_POINTS, wxT("����� �������������� �����"), "TOOL_MBL_POINTS");
    AddRadioTool(MBL_LINES, wxT("����� �������������� ���������������� �������"), "TOOL_MBL_LINES");

    toolBar->AddSeparator();
    AddCheckTool(MODE_SELECT, wxT("����� ���������"), "TOOL_MBL_SELECT");

    toolBar->AddSeparator();
    AddTool(SCALE_MORE, wxT("��������� �������"), "TOOL_SCALE_MORE");
    AddTool(SCALE_LESS, wxT("��������� �������"), "TOOL_SCALE_LESS");
    AddTool(SCALE_REGION, wxT("�������� ���������� �������"), "TOOL_SCALE_REGION");

    toolBar->AddSeparator();
    AddTool(CHANGE_TYPE_GRID, wxT("��������� ����������� ����� ����� - ��������"), "TOOL_GRID_PERCENTS");

    toolBar->Realize();
}


void Frame::AddTool(int id, const wxString &label, const char *nameResource, const char *nameResourceDisabled)
{
    wxBitmap bitmap(nameResource, wxBITMAP_TYPE_BMP_RESOURCE);

    wxBitmap bitmapDisabled(nameResourceDisabled ? wxBitmap(nameResourceDisabled, wxBITMAP_TYPE_BMP_RESOURCE) : bitmap);

    toolBar->AddTool(id, label, bitmap, bitmapDisabled, wxITEM_NORMAL, label, label);

}


void Frame::AddRadioTool(int id, const wxString &label, const char *nameResource)
{
    wxBitmap bitmap(nameResource, wxBITMAP_TYPE_BMP_RESOURCE);

    toolBar->AddRadioTool(id, label, bitmap, bitmap, label, label);
}


void Frame::AddCheckTool(int id, const wxString &label, const char *nameResource)
{
    wxBitmap bitmap(nameResource, wxBITMAP_TYPE_BMP_RESOURCE);

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

        menuAlign->Append(ALIGN_LEFT, "�����");
        menuAlign->Append(ALIGN_RIGHT, "������");
        menuAlign->Append(ALIGN_LEFT_TOP, "����� ������");
        menuAlign->Append(ALIGN_LEFT_DOWN, "����� �����");
        menuAlign->Append(ALIGN_RIGHT_TOP, "������ ������");
        menuAlign->Append(ALIGN_RIGHT_DOWN, "������ �����");

        menuPoint.Append(CONTEXT_MENU_PARAMETERS, "��������");
        menuPoint.AppendSubMenu(menuAlign, "���������");
        menuPoint.Append(CONTEXT_MENU_DELETE, "�������");

        menuContext.Append(FILE_NEW, "��������");
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
    wxFileDialog saveDialog(nullptr, wxT("���������"), wxEmptyString, wxEmptyString, wxT("*.sig"), wxFD_SAVE);

    if (saveDialog.ShowModal() == wxID_OK)
    {
        wxString fileName = saveDialog.GetPath();
        
        wxTextFile file(fileName);

        if(file.Exists())
        {
            wxMessageDialog message(this, wxT("���� � ����� ������ ��� ����������. ������������?"), wxMessageBoxCaptionStr, wxOK | wxCANCEL | wxCENTRE);
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
        toolBar->SetToolNormalBitmap(CHANGE_TYPE_GRID, wxBITMAP(TOOL_GRID_PERCENTS));
        toolBar->SetToolLongHelp(CHANGE_TYPE_GRID, wxT("��������� ����������� ����� ����� - ��������"));
        toolBar->SetToolShortHelp(CHANGE_TYPE_GRID, wxT("��������� ����������� ����� ����� - ��������"));
    }
    else
    {
        toolBar->SetToolNormalBitmap(CHANGE_TYPE_GRID, wxBITMAP(TOOL_GRID_POINTS));
        toolBar->SetToolLongHelp(CHANGE_TYPE_GRID, wxT("��������� ����������� ����� ����� - �����"));
        toolBar->SetToolShortHelp(CHANGE_TYPE_GRID, wxT("��������� ����������� ����� ����� - �����"));
    }

    UpdateStatusBar();
}


void Frame::UpdateStatusBar()
{
    GetStatusBar()->PushStatusText(wxString::Format(wxT("�����: %s"), Grid::GetScale()), 1);

    GetStatusBar()->PushStatusText(wxString::Format(wxT("�������:  %d%%"), Zoomer::Scale()), 2);

    GetStatusBar()->Refresh();
}
