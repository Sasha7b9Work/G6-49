#include "History.h"
#include <vector>


/// Предыдущие элементы
std::vector<Form> pred;
/// Слеюдующие элементы
std::vector<Form> next;


void History::Add(Form* form)
{
	pred.push_back(*form);
	next.clear();
}


Form* History::Prev()
{
	if (pred.size())
	{
		next.push_back(pred.back());
		pred.pop_back();

		return (pred.size()) ? &pred.back() : nullptr;
	}

	return nullptr;
}

Form* History::Next()
{
	if (next.size())
	{
		static Form form = next.back();

		pred.push_back(next.back());
		next.pop_back();

		return &form;
	}

	return nullptr;
}
