#ifndef LIS_TIME_DIA_HPP_891639
#define LIS_TIME_DIA_HPP_891639

#include <iostream>
#include <set>
#include <vector>

#include <lis/iostream.hpp>
#include <lis/type.hpp>





namespace lis { namespace td {





/*
 * Структура, представляющая из себя интервал;
 * может хранить произвольные данные (либо не
 * хранить ничего, тогда в качестве шаблонного
 * параметра необходимо указать void)
 */
template<typename T>
struct Interval
{
	typedef T value_type;

	int beg, end;
	value_type value;

	template<class Ostream>
	Ostream &print( Ostream &os ) const
	{
		os << "(" << beg << ", " << end << ": " << value << ")";
		return os;
	}

	template<typename U>
	bool operator<(Interval<U> const &rhs) const
	{
		return beg < rhs.beg;
	}
};

template<>
struct Interval<void>
{
	typedef void value_type;

	int beg, end;

	template<class Ostream>
	Ostream &print( Ostream &os ) const
	{
		os << "(" << beg << ", " << end << ")";
		return os;
	}

	template<typename U>
	bool operator<(Interval<U> const &rhs) const
	{
		return beg < rhs.beg;
	}
};

template<class Ostream, typename T>
inline Ostream &operator<<( Ostream &os, Interval<T> const &toprint )
{
	toprint.print(os);
	return os;
}

/*
 * Тип, представляющий линию
 */
template<typename T>
using Line = std::multiset<Interval<T>>;





/*
 * Проверяет, пересекаются ли интервалы
 */
template<typename T>
bool crossed(Interval<T> const &lhs, Interval<T> const &rhs)
{
	return lhs < rhs ?
		lhs.end >= rhs.beg :
		rhs.end >= lhs.beg;
}

/*
 * Проверяет, можно ли вставить интервал
 * в данную строку
 */
template<typename T>
bool fits(Interval<T> const &inter, Line<T> const &line)
{
	auto it = line.lower_bound(inter);

	if(it != line.end() && crossed(*it, inter))
		return false;

	if(it == line.begin() || line.empty())
		return true;

	return !crossed(*--it, inter);
}

/*
 * Вывод строки в текстовом ввиде
 */
template<class Ostream, typename T>
void print(Ostream &os, Line<T> const &line)
{
	int c = 0;
	for(auto b = line.begin(), e = line.end(); b != e; ++b)
	{
		for(int i = 0; i < b->beg - c; ++i)
			os.put(' ');
		os.put('*');
		for(int i = b->beg+1; i < b->end; ++i)
			os.put('=');
		if(b->beg != b->end)
			os.put('*');
		c = b->end+1;
	}
	return;
}

/*
 * Структура, которая представляет из себя
 * иерархию интервалов
 */
template<typename T>
struct INode
{
	typedef T value_type;

	INode(bool leaf): leaf(leaf) {}

	INode(Interval<T> const &in):
		leaf(true), in(in) {}

	INode(std::vector<INode> &&ins):
		leaf(false), ins(std::move(ins)) {}

	INode(std::vector<INode> const &ins):
		leaf(false)
	{
		this->ins.resize(ins.size());
		std::copy( ins.begin(), ins.end(), this->ins.begin() );
		return;
	}

	bool operator<(INode const &rhs) const
	{
		if(leaf && rhs.leaf)
			return in < rhs.in;

		if(leaf || rhs.leaf)
			return leaf;

		return ins[0] < rhs.ins[0];
	}

	bool leaf = true;
	Interval<T> in;
	std::vector<INode> ins;
};






template<typename T>
struct TimeDia
{
	typedef T value_type;

	std::vector<Line<T>> field;
	int min = -1, max = -1;



	// methods
	/*
	 * Добавить узел (может быть как листом, т.е.
	 * конечным интервалом, так и группой TimeDia)
	 */
	void push(INode<T> const &node)
	{
		if(node.leaf)
			return push(node.in);
		TimeDia dia;
		for(auto b = node.ins.begin(), e = node.ins.end(); b != e; ++b)
			dia.push(*b);
		push(dia);
		return;
	}

	void push(Interval<T> const &inter)
	{
		for(auto b = field.begin(), e = field.end(); b != e; ++b)
		{
			if(fits(inter, *b))
			{
				b->insert(inter);
				return;
			}
		}

		field.push_back( Line<T>() );
		field.back().insert(inter);
		return;
	}

	void push(TimeDia const &dia)
	{
		int off = 0;
		Interval<T> trial;

	again:
		for(int i = 0; i < (int)dia.field.size(); ++i)
		{
			if(dia.field[i].empty())
				continue;

			trial.beg = dia.field[i].begin()->beg;
			trial.end = prev(dia.field[i].end())->end;

			if(off + i == (int)field.size())
				field.push_back( Line<T>() );

			if(!fits(trial, field[off+i]))
				goto continue_mainloop_label;
		}

		for(int i = 0; i < (int)dia.field.size(); ++i)
		{
			for(auto b = dia.field[i].begin(), e = dia.field[i].end(); b != e; ++b)
				field[off+i].insert(*b);
		}

		return;

	continue_mainloop_label:
		++off;
		goto again;
	}


	/*
	 * Найти границы, в которых расположены все интервалы
	 */
	void calculate_bounds()
	{
	 	min = int_max;
		max = int_min;

		for(auto b = field.begin(), e = field.end(); b != e; ++b)
		for(auto bb = b->begin(), ee = b->end(); bb != ee; ++bb)
		{
			min = std::min(min, bb->beg);
			max = std::max(max, bb->end);
		}

		return;
	}



	/*
	 * Вывести в виде строки
	 */
	template<class Ostream>
	Ostream &print( Ostream &os ) const
	{
		for(auto b = field.begin(), e = field.end(); b != e; ++b)
		{
			if(b != field.begin())
				os.put('\n');
			print(os, *b);
		}

		return os;
	}
};





} }





#endif
