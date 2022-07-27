// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "History.h"
#include <vector>


// ���������� ��������
static std::vector<Form> pred;
// ���������� ��������
static std::vector<Form> next;


void History::Add(const Form* form)
{
    TheCanvas->Redraw();

    if(pred.empty() || !pred[pred.size() - 1].IsEquals(form))
    {
        pred.push_back(*form);
    }
}


Form* History::Prev()
{
    TheCanvas->Redraw();

	if (pred.size() > 1)
	{
		next.push_back(pred.back());
		pred.pop_back();
	}

	return pred.size() ? &pred.back() : nullptr;
}

Form* History::Next()
{
    TheCanvas->Redraw();

    if (next.size())
	{
		pred.push_back(next.back());
		next.pop_back();
	}

	return pred.size() ? &pred.back() : nullptr;
}
