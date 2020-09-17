#pragma once
#pragma warning(push, 0)
#include "wx/wx.h"
#pragma warning(pop)


class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);
    ~Frame();

    void OnQuit(wxCommandEvent &);
    void OnTimer(wxTimerEvent &);
    void OnRepaint(wxPaintEvent &);

    // Покзать контекстное меню в позиции pos
    void ShowContextMenu(const wxPoint &pos);

    // Заблокировать/разблокировать холст (при заблокированном холсте нельзя ставить точки мышкой)
    void SetBlockingCanvas(bool blocking);

    // Возвращает true, если холст заблокирован (нельзя ставить точки мышкой)
    bool IsBlockingCanvas() { return isBlockingCanvas; }

    void SetModeMBL();

    void UpdateStatusBar();

private:
    wxTimer timer;

    wxToolBar *toolBar = nullptr;

    bool isBlockingCanvas = false;  // Если true - нельзя ставить точки мышкой

    void SetSizeAndPosition();

    wxRect GetMaxDisplay();

    void CreateMenu();

    void AddTool(int id, const wxString &label, const char *nameResource, const char *nameResourceDisabled = nullptr);

    void AddRadioTool(int id, const wxString &label, const char *nameResource);

    void AddCheckTool(int id, const wxString &label, const char *nameResource);

    void OnAlignLeft(wxCommandEvent &);

    void OnAlignRight(wxCommandEvent &);

    void OnAlignLeftTop(wxCommandEvent &);

    void OnAlignLeftDown(wxCommandEvent &);

    void OnAlignRightTop(wxCommandEvent &);

    void OnAlignRightDown(wxCommandEvent &);

    void OnDeleteMarker(wxCommandEvent &);

    void OnParametersPoint(wxCommandEvent &);

	void OnUndo(wxCommandEvent &);

	void OnRedo(wxCommandEvent &);

    void OnOpenFile(wxCommandEvent &);

    void OnSaveFile(wxCommandEvent &);

    void OnNewFile(wxCommandEvent &);

    void CreateTriangle(wxCommandEvent &);

    void CreateTrapeze(wxCommandEvent &);

    void CreateExponent(wxCommandEvent &);

    void CreateSinX(wxCommandEvent &);

    void CreateGauss(wxCommandEvent &);

    void CreateGaversine(wxCommandEvent &);

    void CreateNoise(wxCommandEvent &);

    void CreateComposite(wxCommandEvent &);

    void InsertPoints(wxCommandEvent &);

    void OnKeyDown(wxKeyEvent &);

    // Установить режим выделения точек ЛКМ
    void SetPointsMBL(wxCommandEvent &);

    // Установить режим выделения линий ЛКМ
    void SetLinesMBL(wxCommandEvent &);

    // Включить/выключить режим выделения
    void SetModeSelect(wxCommandEvent &);

    void OnScaleMore(wxCommandEvent &);

    void OnScaleLess(wxCommandEvent &);

    void OnScaleRegion(wxCommandEvent &);

    void OnChangeTypeGrid(wxCommandEvent &);
};


extern Frame *TheFrame;
