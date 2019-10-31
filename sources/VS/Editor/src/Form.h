#pragma once
#include "defines.h"
#include "Canvas.h"
#include "MyMath.h"
#include <vector>

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

	static const uint16 MAX_VALUE = static_cast<uint16>((1 << 12) - 1);
	static const uint16 AVE_VALUE = static_cast<uint16>((MAX_VALUE) / 2);

	static const uint NUM_POINTS = 8192;

	Point(int mouseX, int mouseY)
	{
		pos = MyMath::Round<uint16>(mouseX / ScaleX());
		data = MyMath::Round<uint16>(mouseY / ScaleY());
	}
	Point(uint16 p, uint16 d) : pos(p), data(d) {};
	void SetY(int mouseY)
	{
		data = MyMath::Round<uint16>(mouseY / ScaleY());
	}

	uint16 pos;
	uint16 data;
	bool operator < (const Point& point) const
	{
		return (pos < point.pos);
	}
	/// Возвращает true, если курсор мыши находится над этой точкой
	bool UnderMouse(int mouseX, int mouseY)
	{
		return (MyMath::Abs(mouseX - static_cast<int>(pos)) <= SIZE * 5) && (MyMath::Abs(mouseY - static_cast<int>(data)) <= SIZE * 5);
	}
	/// Масштаб по горизонтали
	static float ScaleX()
	{
		return TheCanvas->GetSize().x / static_cast<float>(NUM_POINTS);
	}
	/// Масштаб по вертикали
	static float ScaleY()
	{
		return TheCanvas->GetSize().y / static_cast<float>(MAX_VALUE);
	}
};


class Form
{
public:
    Form();
    /// Очистить сигнал
    void Clear();
    /// Добавить точку с координатами мыши
    void SetPoint(int mouseX, int mouseY);
    /// Добавить точку с абсолютными значениями
    void SetPoint(uint16 pos, uint16 data);
    /// Возвращает true, если курсор мыши находится над поставленной точкой. Pressed - нажата ли кнопка мыши
    bool ExistPoint(int mouseX, int mouseY, bool pressed);
    /// Удалить точку в позиции мыши
    void RemovePoint();
    /// Переместить точку в новую позицию
    void MovePoint(int mouseX, int mouseY);
    /// Выровнять точку
    void AlignPoint(Align::E align);

    void Draw();

	void UndoHistory();

	void RedoHistory();

    void CreateSine();

private:

	/// Данные, готовые для передачи в прибор
	uint16 data[Point::NUM_POINTS];

	std::vector<Point> points;
	/// Здесь хранится индекс точки, которой управляем в текущий момент
	uint iCurPoint = 0;
	/// Возвращает index точки в позиции pos. 0xFFFFFFFF, если точки в этой позиции нет
	uint PointInPosition(uint16 pos);
	/// Заносит точку куда следует
	void SetPoint(Point point);
	/// Рассчитать соседние с point точки
	void CalculateNeighboringPoints(const Point& point);
	/// Линейно интерполировать точки, расположенные между pos1 и pos2
	void LinearInterpolation(uint16 pos1, uint16 pos2);
	/// Интерполировать точки слева от точки с индексом index из points
	void LinearInterpolationLeft(uint index);
	/// Интерполировать точки справа от точки с индексом index из points
	void LinearInterpolationRight(uint index);
};


extern Form *TheForm;
