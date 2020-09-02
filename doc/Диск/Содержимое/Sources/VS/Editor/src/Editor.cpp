#include "Editor.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/display.h>
#include <wx/mstream.h>
#include <wx/textfile.h>
#pragma warning(pop)

#undef main


#include "defines.h"
#include "Canvas.h"
#include "Form.h"
#include "History.h"
#include "Dialogs/ExponentDialog.h"
#include "Dialogs/GaussDialog.h"
#include "Dialogs/InsertPointsDialog.h"
#include "Dialogs/ParametersDialog.h"
#include "Dialogs/SinXDialog.h"
#include "Dialogs/TrapezeDialog.h"
#include "Dialogs/TriangleDialog.h"
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
    Bind(wxEVT_MENU,     &Frame::OnDeletePoint,     this, CONTEXT_MENU_DELETE);
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
    Bind(wxEVT_MENU,     &Frame::InsertPoints,      this, INSERT_POINTS);
    Bind(wxEVT_TIMER,    &Frame::OnTimer,           this, TIMER_ID);
    Bind(wxEVT_PAINT,    &Frame::OnRepaint,         this);
    Bind(wxEVT_KEY_DOWN, &Frame::OnKeyDown,         this);

    Show(true);

    timer.Start(0);

    TheFrame = this;
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

    wxBitmap imgOpen(wxImage(wxT("icons/open.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgSave(wxImage(wxT("icons/save.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgNew(wxImage(wxT("icons/new.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgUndo(wxImage(wxT("icons/undo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgRedo(wxImage(wxT("icons/redo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateTriangle(wxImage(wxT("icons/triangle.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateTrapeze(wxImage(wxT("icons/trapeze.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateExponent(wxImage(wxT("icons/exponent.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateSinX(wxImage(wxT("icons/sinx.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateGauss(wxImage(wxT("icons/gauss.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateGaversine(wxImage(wxT("icons/gaversine.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateNoise(wxImage(wxT("icons/noise.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateComposite(wxImage(wxT("icons/composite.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgInsertPoints(wxImage(wxT("icons/points.bmp"), wxBITMAP_TYPE_BMP));

    toolBar = CreateToolBar();
    toolBar->AddTool(FILE_OPEN, wxT("Открыть"), imgOpen, wxT("Загрузить ранее созданный сигнал из файла"));
    toolBar->AddTool(FILE_SAVE, wxT("Сохранить"), imgSave, wxT("Сохранить сигнал в файл"));
    toolBar->AddTool(FILE_NEW, wxT("Новый"), imgNew, wxT("Создать новый сигнал"));

    toolBar->AddSeparator();
    toolBar->AddTool(UNDO, wxT("Отменить"), imgUndo, wxT("Отменить предыдущее действие"));
    toolBar->AddTool(REDO, wxT("Восстановить"), imgRedo, wxT("Восстановить следующее действие"));

    toolBar->AddSeparator();
    toolBar->AddTool(CREATE_TRIANGLE, wxT("Треугольник"), imgCreateTriangle, wxT("Создать новый сигнал в форме треугольника"));
    toolBar->AddTool(CREATE_TRAPEZE, wxT("Трапеция"), imgCreateTrapeze, wxT("Создать новый сигнал в форме трапеции"));
    toolBar->AddTool(CREATE_EXPONENT, wxT("Експонента"), imgCreateExponent, wxT("Создать новый экспоненциальный сигнал"));
    toolBar->AddTool(CREATE_SINX, wxT("Sin(x)/x"), imgCreateSinX, wxT("Создать сигнал вида sin(x)/x"));
    toolBar->AddTool(CREATE_GAUSS, wxT("Гауссова функция"), imgCreateGauss, wxT("Создать новый сигнал в виде гуссовой функции"));
    toolBar->AddTool(CREATE_GAVERSINE, wxT("Гаверсинус"), imgCreateGaversine, wxT("Создать новый сигнал в форме гаверсинуса"));
    toolBar->AddTool(CREATE_NOISE, wxT("Шум"), imgCreateNoise, wxT("Создать шумовой сигнал"));
    toolBar->AddTool(CREATE_COMPISITE, wxT("Сложный сигнал"), imgCreateComposite, wxT("Создать сложный сигнал из гармоник"));

    toolBar->AddSeparator();
    toolBar->AddTool(INSERT_POINTS, wxT("Вставить точки"), imgInsertPoints, wxT("Вставить маркеры"));
    toolBar->Realize();
}


void Frame::ShowContextMenu(const wxPoint &pos)
{
    bool underPoint = TheForm->ExistPoint(pos.x, pos.y, false, &indexPoint, &valuePoint);

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


void Frame::OnDeletePoint(wxCommandEvent &)
{
    TheForm->RemovePoint();
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

//void Frame::CreateSine(wxCommandEvent &)
//{
//    TheFrame->SetBlockingCanvas(true);
//
//    static uint16 data[Point::NUM_POINTS];
//
//    for (int i = 0; i < Point::NUM_POINTS; i++)
//    {
//        data[i] = static_cast<uint16>(Point::AVE + (std::sinf(static_cast<float>(i) / Point::NUM_POINTS * 2.0F * 3.14F) * Point::AVE));
//    }
//
//    TheForm->SetMainForm(data, nullptr);
//}

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
    static uint16 data[Point::NUM_POINTS];

    for (int i = 0; i < Point::NUM_POINTS; i++)
    {
        float sinx = std::sinf(static_cast<float>(i) / Point::NUM_POINTS * 2.0F * 3.14F);

        data[i] = static_cast<uint16>(Point::AVE + static_cast<uint16>(sinx * sinx * Point::AVE));
    }

    TheFrame->SetBlockingCanvas(true);

    TheForm->SetMainForm(data, nullptr);

//    for (int i = 0; i < Point::NUM_POINTS; i++)
//    {
//        float sinx = std::sinf(static_cast<float>(i + Point::NUM_POINTS / 8 * 7) / Point::NUM_POINTS * 2.0F * 3.14F * 2.0F);
//
//        data[i] = static_cast<uint16>(Point::AVE + sinx * Point::AVE);
//    }
//
//    TheForm->SetAdditionForm(data);
}


void Frame::CreateNoise(wxCommandEvent &)
{
    static uint16 data[Point::NUM_POINTS];

    for (int i = 0; i < Point::NUM_POINTS; i++)
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


void Frame::OnKeyDown(wxKeyEvent &)
{
//    if(event.GetKeyCode() == 'x' || event.GetKeyCode() == 'X')
//    {
//        if(event.ControlDown())
//        {
//            Close(true);
//        }
//    }
}
