#pragma once
#include "defines.h"
#include "Canvas.h"
#include "MyMath.h"
#include <vector>
#pragma warning(push, 0)
#include <wx/textfile.h>
#pragma warning(pop)

/*
    Класс представления формы сигнала
*/


struct Align
{
    enum E
    {
        Left,
        Right,
        LeftTop,
        LeftDown,
        RightTop,
        RightDown
    };
};


struct Point
{
	static const uint SIZE = 5;

    static const uint16 MIN = static_cast<uint16>(0);
	static const uint16 MAX = static_cast<uint16>((1 << 12) - 1);
	static const uint16 AVE = static_cast<uint16>((MAX) / 2);

	static const uint NUM_POINTS = 8192;

	Point(int mouseX, int mouseY)
	{
		pos = MyMath::Round<uint16>(static_cast<float>(mouseX) / ScaleX());
		data = static_cast<uint16>(Point::MAX - MyMath::Round<uint16>(static_cast<float>(mouseY) / ScaleY()));
	}
	Point(uint16 p, uint16 d) : pos(p), data(d) {};
	void SetY(int mouseY)
	{
		data = static_cast<uint16>(Point::MAX - MyMath::Round<uint16>(static_cast<float>(mouseY) / ScaleY()));
	}

	uint16 pos;
	uint16 data;
	bool operator < (const Point& point) const
	{
		return (pos < point.pos);
	}
	/// Возвращает true, если курсор мыши находится над этой точкой
	double DistanceFromMouse(int mouseX, int mouseY)
	{
        int dX = mouseX - static_cast<int>(pos);

        int dY = mouseY - static_cast<int>(Point::MAX - data);

        return std::sqrt(dX * dX + dY * dY);
	}
	/// Масштаб по горизонтали
	static float ScaleX()
	{
		return static_cast<float>(TheCanvas->GetSize().x) / static_cast<float>(NUM_POINTS);
	}
	/// Масштаб по вертикали
	static float ScaleY()
	{
		return static_cast<float>(TheCanvas->GetSize().y) / static_cast<float>(MAX);
	}
};


class Form
{
public:
    Form();
    ~Form();
    // Очистить сигнал
    void Clear();

	// Добавить точку с координатами мыши
    void SetPoint(int mouseX, int mouseY);

	// Добавить точку с абсолютными значениями
    void SetPoint(uint16 pos, uint16 data);

	// Заносит точку куда следует
    void SetPoint(Point point);

	// Вставить точку в позицию pos
    void SetPoint(uint16 pos);

	// Возвращает true, если курсор мыши находится над поставленной точкой. Pressed - нажата ли кнопка мыши
	bool ExistPoint(int mouseX, int mouseY, bool pressed, uint16 *index = nullptr, uint16 *value = nullptr);

	// Удалить точку в позиции мыши
    void RemovePoint();

	// Переместить точку в новую позицию
    void MovePoint(int mouseX, int mouseY);

	// Выровнять точку
    void AlignPoint(Align::E align);

    void Draw();

	void UndoHistory();

	void RedoHistory();

	// Установить дополнительную форму, которая будет рисоваться поверх основной. 
    void SetAdditionForm(const uint16 data[Point::NUM_POINTS]);

	// Установить основную форму
    void SetMainForm(const uint16 data[Point::NUM_POINTS], const std::vector<Point> *points);

    bool IsEquals(const Form *form) const;

    void SaveToFile(wxTextFile &file);

    void LoadFromFile(wxTextFile &file);

private:

	// Данные, готовые для передачи в прибор
	uint16 data[Point::NUM_POINTS];

	// Точки, поставленные мышью. Хранятся в системе координат прибора (pos = [0 ... Point::NUM_POINTS], data = [Point::MIN ... Point::MAX])
	std::vector<Point> points;

	// Здесь хранится индекс точки, которой управляем в текущий момент
	uint iCurPoint = 0;

	// Возвращает index точки в позиции pos. 0xFFFFFFFF, если точки в этой позиции нет
	uint PointInPosition(uint16 pos);

	// Рассчитать соседние с point точки
	void CalculateNeighboringPoints(const Point& point);

	// Линейно интерполировать точки, расположенные между pos1 и pos2
	void LinearInterpolation(uint16 pos1, uint16 pos2);

	// Интерполировать точки слева от точки с индексом index из points
	void LinearInterpolationLeft(uint index);

	// Интерполировать точки справа от точки с индексом index из points
	void LinearInterpolationRight(uint index);
};


extern Form *TheForm;
