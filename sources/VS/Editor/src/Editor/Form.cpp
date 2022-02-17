#include "defines.h"
#include "Form.h"
#include "Editor/Editor.h"
#include "Editor/History.h"
#include "Editor/Painter/Canvas.h"
#include "Editor/Painter/Painter.h"

#pragma warning(push, 0)
#include <wx/msgdlg.h>
#pragma warning(pop)


Form *TheForm = nullptr;


static uint16 addData[Point::AMOUNT];
static bool drawAdditionData = false;

#define LABEL_FILE  wxT("Data file G6-49")


// ����������� ������ � ��� ����� ����� � ���������� true � �������� ������
static bool ConvertStringToTwoShort(const wxString &line, uint16 *val1, uint16 *val2);
// ����������� ��������� � ����� �����. ���������� false � ������ �������
static bool ConvertSubStringToShort(const wxString &line, uint startPos, uint endPos, uint16 *value, unsigned long max);
// ����� ��������� �� ������
static void ErrorMessage(const wxString &text);
static void ErrorMessage(uint numString);


bool operator==(const Point &left, const Point &right)
{
    return (left.pos == right.pos) && (left.data == right.data);
}


Form::Form()
{
    Clear();
}


Form::~Form()
{
    markers.clear();
}


void Form::Clear()
{
    for (int i = 0; i < Point::AMOUNT; i++)
    {
        data[i] = Point::AVE;
    }

    markers.clear();

    SetPointInRealCoord(static_cast<uint16>(0), Point::AVE);
    SetPointInRealCoord(static_cast<uint16>(Point::AMOUNT - 1), Point::AVE);
}


uint Form::PointInPosition(uint16 pos)
{
    for (uint i = 0; i < markers.size(); i++)
    {
        if (markers[i].pos == pos)
        {
            return i;
        }
    }

    return static_cast<uint>(-1);
}


void Form::SetPoint(Point point)
{
    data[point.pos] = point.data;

    uint index = PointInPosition(point.pos);

    if (index == static_cast<uint>(-1))
    {
        markers.push_back(point);

        std::sort(markers.begin(), markers.end());
    }
    else
    {
        markers[index] = point;
    }

    CalculateNeighboringPoints(point);
}


void Form::SetMarkerInMouseCoord(int mouseX, int mouseY)
{
    if (mouseY < Grid::Y())
    {
        return;
    }

    if (!TheFrame->IsBlockingCanvas())
    {
        SetPoint(Point::FromCanvas(mouseX, mouseY));
        History::Add(TheForm);
    }
}


void Form::UndoHistory()
{	
	Form* form = History::Prev();

	if (form)
	{
		*TheForm = *form;
	}
}


void Form::RedoHistory()
{
	Form* form = History::Next();

	if (form)
	{
		*TheForm = *form;
	}
}


void Form::SetPointInRealCoord(uint16 pos, uint16 dat)
{
    if (!TheFrame->IsBlockingCanvas())
    {
        SetPoint(Point::FromData(pos, dat));
    }
}


void Form::SetPointInPosition(uint16 pos)
{
    Point point = Point::FromData(pos, data[pos]);

    uint index = PointInPosition(point.pos);

    if(index != static_cast<uint>(-1))
    {
        markers[index] = point;
    }
    else
    {
        markers.push_back(point);

        std::sort(markers.begin(), markers.end());
    }
}


void Form::RemoveCurrentMarker()
{
    if (iCurMarker < markers.size())
    {
        markers.erase(markers.begin() + static_cast<const int>(iCurMarker));

        LinearInterpolationLeft(iCurMarker);

        iCurMarker = static_cast<uint>(-1);
    }
}


void Form::MoveMarker(int canvasX, int canvasY)
{
    if (iCurMarker == 0)
    {
        markers[0].SetCanvasY(canvasY);
        data[0] = markers[0].data;
        LinearInterpolationRight(0);
    }
    else if (iCurMarker == markers.size() - 1)
    {
        markers[iCurMarker].SetCanvasY(canvasY);
        data[Point::AMOUNT - 1] = markers[iCurMarker].data;
        LinearInterpolationLeft(iCurMarker);
    }
    else
    {
        Point point = Point::FromCanvas(canvasX, canvasY);

        Point left = markers[iCurMarker - 1];

        Point right = markers[iCurMarker + 1];

        if (point.pos <= left.pos)
        {
            point.pos = static_cast<uint16>(left.pos + 1);
        }
        else if (point.pos >= right.pos)
        {
            point.pos = static_cast<uint16>(right.pos - 1);
        }

        uint tempIndex = iCurMarker;

        RemoveCurrentMarker();

        iCurMarker = tempIndex;

        SetPoint(point);
    }

    TheCanvas->Redraw();
}


static bool CompareLess(Point left, Point right)
{
    return left.data < right.data;
}


static bool CompareMore(Point left, Point right)
{
    return left.data > right.data;
}


typedef bool (*pFuncCompare)(Point, Point);


void Form::AlignPoint(Align::E align)
{
    uint index = static_cast<uint>(-1);

    Point point = markers[iCurMarker];

    if (align == Align::Left || align == Align::LeftTop || align == Align::LeftDown)
    {
        if (iCurMarker > 0)
        {
            if(align == Align::Left)
            {
                index = iCurMarker - 1;
            }
            else
            {
                uint i = iCurMarker - 1;

                pFuncCompare func = (align == Align::LeftDown) ? CompareLess : CompareMore;

                while (i < markers.size())
                {
                    if(func(markers[i], point))
                    {
                        index = i;
                        break;
                    }
                    i--;
                }
            }
        }
    }
    else
    {
        if (iCurMarker < markers.size() - 1)
        {
            if (align == Align::Right)
            {
                index = iCurMarker + 1;
            }
            else
            {
                uint i = iCurMarker + 1;

                pFuncCompare func = (align == Align::RightDown) ? CompareMore : CompareLess;

                while (i < markers.size() - 1)
                {
                    if (func(point, markers[i]))
                    {
                        index = i;
                        break;
                    }
                    i++;
                }
            }
        }
    }

    if (index != static_cast<uint>(-1))
    {
        markers[iCurMarker].data = markers[index].data;
        SetPoint(markers[iCurMarker]);
    }

    History::Add(TheForm);
}


int Point::CanvasX() const
{
    return Math::Round<int>(PixelsInPointX() * static_cast<float>(pos - Zoomer::IndexFirsPoint()));
}


int Point::CanvasY() const
{
    return Grid::Y() + Math::Round<int>(PixelsInDiscretY() * static_cast<float>(Point::MAX - data));
}


bool Form::ExistMarker(int canvasX, int canvasY, bool pressed, uint16 *index, uint16 *value)
{
    if (canvasY < Grid::Y())
    {
        return false;
    }

    uint16 positionNearestPoint = static_cast<uint16>(-1);
    double nearestDistance = 1e10;

    for(uint16 i = 0; i < markers.size(); i++)
    {
        double distance = markers[i].DistanceFromCanvas(canvasX, canvasY);

        if(distance < nearestDistance)
        {
            nearestDistance = distance;
            positionNearestPoint = i;
        }
    }

    if(nearestDistance < Marker::SIZE * 2.5)
    {
        if(iCurMarker == static_cast<uint>(-1) || !pressed)
        {
            iCurMarker = positionNearestPoint;
        }

        if (index)
        {
            *index = markers[positionNearestPoint].pos;
        }

        if (value)
        {
            *value = markers[positionNearestPoint].data;
        }

        return true;
    }

    if (!pressed)
    {
        iCurMarker = static_cast<uint>(-1);
    }

    return false;
}


void Form::CalculateNeighboringPoints(const Point &point)
{
    uint index = 0;                     // ����� ����� ������ point � ������� points

    for (; index < markers.size(); index++)
    {
        if (point == markers[index])
        {
            break;
        }
    }

    LinearInterpolationLeft(index);

    LinearInterpolationRight(index);
}


static void DrawData(const uint16 data[Point::AMOUNT], const Color &colorBright, const Color &colorDark)
{
    if (Point::PixelsInPointX() > 3.0F)
    {
        for (int i = Zoomer::IndexFirsPoint() + 1; i < Zoomer::IndexLastPoint(); i++)
        {
            Point p0 = Point::FromData(static_cast<uint16>(i - 1), data[i - 1]);
            Point p1 = Point::FromData(static_cast<uint16>(i), data[i]);

            Painter::DrawLine(p0.CanvasX(), p0.CanvasY(), p1.CanvasX(), p1.CanvasY(), colorDark);

            Painter::DrawPoint(p1.CanvasX(), p1.CanvasY(), Point::SIZE, colorBright);
        }
    }
    else
    {
        Painter::SetColor(colorBright);

        for (int i = Zoomer::IndexFirsPoint() + 1; i < Zoomer::IndexLastPoint(); i++)
        {
            Point p0 = Point::FromData(static_cast<uint16>(i - 1), data[i - 1]);
            Point p1 = Point::FromData(static_cast<uint16>(i), data[i]);

            Painter::DrawLine(p0.CanvasX(), p0.CanvasY(), p1.CanvasX(), p1.CanvasY());
        }
    }
}


void Form::Draw()
{
    DrawData(data, Color::WHITE, Color::GRAY_50);

    Painter::SetColor(Color::GREEN);

    for (Point point : markers)
    {
        Painter::DrawPoint(point.CanvasX(), point.CanvasY(), Marker::SIZE);
    }

    if (iCurMarker != static_cast<uint>(-1))
    {
        Point &point = markers[iCurMarker];
        Painter::DrawPoint(point.CanvasX(), point.CanvasY(), Marker::SIZE * 3);
        point.DrawParameters();
    }

    if (drawAdditionData)
    {
        DrawData(addData, Color::BLUE, Color::BLUE_50);
    }
}


void Form::LinearInterpolation(uint16 pos1, uint16 pos2)
{
    uint16 data1 = data[pos1];
    uint16 data2 = data[pos2];

    float delta = static_cast<float>(data2 - data1) / static_cast<float>(pos2 - pos1);  // �������� �������� ����� ��������� �������
    
    float value = static_cast<float>(data[pos1]) + delta;                                  // �������� � ������� �������
    
    for (int i = pos1 + 1; i < pos2; i++)
    {
        data[i] = Math::Round<uint16>(value);
        value += delta;
    }
}


void Form::LinearInterpolationLeft(uint index)
{
    Point point = markers[index];

    if (index == 0)                     // ���� ����� ����� ������
    {
        if (point.pos > 0)
        {
            LinearInterpolation(0, point.pos);
        }
    }
    else
    {
        LinearInterpolation(markers[index - 1].pos, point.pos);
    }

}


void Form::LinearInterpolationRight(uint index)
{
    Point point = markers[index];

    if (index == markers.size() - 1)
    {
        if (point.pos < Point::AMOUNT - 1)
        {
            LinearInterpolation(point.pos, Point::AMOUNT - 1);
        }
    }
    else
    {
        LinearInterpolation(point.pos, markers[index + 1].pos);
    }
}


void Form::SetMainForm(const uint16 dat[Point::AMOUNT], const std::vector<Point> *p)
{
    for (int i = 0; i < Point::AMOUNT; i++)
    {
        data[i] = Point::AVE;
    }

    markers.clear();

    SetPointInRealCoord(static_cast<uint16>(0), dat[0]);
    SetPointInRealCoord(static_cast<uint16>(Point::AMOUNT - 1), dat[Point::AMOUNT - 1]);

    std::memcpy(data, dat, Point::AMOUNT * 2);

    if(p)
    {
        for(Point point : *p)
        {
            SetPoint(point);
        }
    }

    History::Add(TheForm);
}


void Form::SetAdditionForm(const uint16 d[Point::AMOUNT])
{
    if (d)
    {
        std::memcpy(addData, d, Point::AMOUNT * 2);
        drawAdditionData = true;
    }
    else
    {
        drawAdditionData = false;
    }

    TheCanvas->Redraw();
}


bool Form::IsEquals(const Form *form) const
{
    if(markers != form->markers)
    {
        return false;
    }

    for(int i = 0; i < Point::AMOUNT; i++)
    {
        if(data[i] != form->data[i])
        {
            return false;
        }
    }

    return true;
}


void Form::SaveToFile(wxTextFile &file)
{
    file.Clear();

    file.AddLine(LABEL_FILE);

    file.AddLine(wxT("points"));

    if (markers.size() == 0)
    {
        for (int i = 0; i < Point::AMOUNT; i++)
        {
            file.AddLine(wxString::Format(wxT("%i %i"), i, data[i]));
        }
    }
    else
    {
        for (Point &point : markers)
        {
            file.AddLine(wxString::Format(wxT("%i %i"), point.pos, point.data));
        }
    }
}


void Form::LoadFromFile(wxTextFile &file)
{
    wxString line = file.GetFirstLine();

    if(line.Cmp(LABEL_FILE) != 0)
    {
        return ErrorMessage(wxT("���� �� �������� ������ ������."));
    }

    if(file.GetNextLine().Cmp(wxT("points")) != 0)
    {
        return ErrorMessage(file.GetCurrentLine());
    }

    while(true)
    {
        line = file.GetNextLine();
        if(line.IsEmpty())
        {
            break;
        }

        uint16 index = 0;
        uint16 d = 0;

        if(!ConvertStringToTwoShort(line, &index, &d))
        {
            return ErrorMessage(file.GetCurrentLine());
        }

        SetPointInRealCoord(index, d);
    }
}


static void ErrorMessage(const wxString &text)
{
    wxMessageBox(text);
}


static void ErrorMessage(uint numString)
{
    wxMessageBox(wxString::Format(wxT("������ ������ � ������ %i"), numString));
}


static bool ConvertStringToTwoShort(const wxString &line, uint16 *val1, uint16 *val2)
{
    int pos = line.Find(' ');

    if(pos == -1)
    {
        return false;
    }

    if(!ConvertSubStringToShort(line, 0, static_cast<uint>(pos - 1), val1, Point::AMOUNT))
    {
        return false;
    }

    if(!ConvertSubStringToShort(line, static_cast<uint>(pos + 1), line.size() - 1, val2, Point::MAX))
    {
        return false;
    }

    return true;
}


static bool ConvertSubStringToShort(const wxString &line, uint startPos, uint endPos, uint16 *value, unsigned long max)
{
    wxString str = line.SubString(startPos, endPos);

    unsigned long val = 0;

    if(!str.ToULong(&val) || val > max)
    {
        return false;
    }

    *value = static_cast<uint16>(val);

    return true;
}


void Point::DrawParameters()
{
    wxPoint coord = CalculateCoordParameters();

    Painter::DrawFilledRectangle(coord.x, coord.y, 60, 35, Color::BLACK, Color::WHITE);

    Painter::DrawText(coord.x + 3, coord.y + 2, wxString::Format(wxT("x : %d"), pos));

    Painter::DrawText(coord.x + 3, coord.y + 16, wxString::Format(wxT("y : %d"), data));
}


wxPoint Point::CalculateCoordParameters()
{
    bool smallX = false;

    int x = CanvasX() - 69;

    if (x < Grid::X() + 3)
    {
        x = Grid::X() + 3;
        smallX = true;
    }

    int y = CanvasY() - 44;

    if (y < Grid::Y() + 3)
    {
        y = Grid::Y() + 3;

        if (smallX)
        {
            x = CanvasX() + 15;
            y = CanvasY() + 15;
        }
    }

    return { x, y };
}
