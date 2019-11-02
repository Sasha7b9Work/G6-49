#include "History.h"
#include <vector>


/// Предыдущие элементы
std::vector<Form> pred;
/// Слеюдующие элементы
std::vector<Form> next;


void History::Add(const Form* form)
{
    if(!pred.empty() && pred[pred.size() - 1].IsEquals(form))
    {
        return;
    }

	pred.push_back(*form);
	next.clear();
}


Form* History::Prev()
{
	if (pred.size() > 1)
	{
		next.push_back(pred.back());
		pred.pop_back();
	}

	return pred.size() ? &pred.back() : nullptr;
}

Form* History::Next()
{
	if (next.size())
	{
		pred.push_back(next.back());
		next.pop_back();
	}

	return pred.size() ? &pred.back() : nullptr;
}
