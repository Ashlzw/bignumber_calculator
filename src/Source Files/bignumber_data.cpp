#include "bignumber_data.h"

#include <exception>

std::allocator<char> bignumber_data::alloc;

bignumber_data::bignumber_data(const bignumber_data & bd)
{
	auto p = alloc_n_copy(bd.begin(), bd.end());
	datas = p.first;
	first_free = cap = p.second;
}

bignumber_data::bignumber_data(const char *b, const char *e)
{
	auto p = alloc_n_copy(b, e);
	datas = p.first;
	first_free = cap = p.second;
}

bignumber_data & bignumber_data::operator=(const bignumber_data bd)
{
	swap(bd);
	return *this;
}

char & bignumber_data::operator[](std::size_t n)
{
	return const_cast<char&>(static_cast<const bignumber_data&>(*this)[n]);
}

const char & bignumber_data::operator[](std::size_t n) const
{
	if (n < size())
	{
		return datas[n];
	}
	else
		throw std::out_of_range("index out of range.");
}

void bignumber_data::push_back(const char c)
{
	chk_n_alloc();
	alloc.construct(first_free++, c);
}

void bignumber_data::free()
{
	if (datas)
	{
		for (char *i = datas; i != cap; ++i)
			alloc.destroy(i);
		alloc.deallocate(datas, cap - datas);
	}
}

std::pair<char*, char*> bignumber_data::alloc_n_copy(const char * b, const char * e)
{
	char *newdatas = alloc.allocate(e - b);
	return std::make_pair(newdatas, std::uninitialized_copy(b, e, newdatas));
}

void bignumber_data::reallocate()
{
	size_t newcapacity = size() ? 2 * size() : 1;

	char *newdatas = alloc.allocate(newcapacity);

	char *dest = newdatas;
	char *elem = datas;

	for (size_t i = 0; i < size(); ++i)
		alloc.construct(dest++, *elem++);

	free();

	datas = newdatas;
	first_free = dest;
	cap = datas + newcapacity;
}

void bignumber_data::swap(const bignumber_data & bd)
{
	auto p = alloc_n_copy(bd.begin(), bd.end());
	free();
	datas = p.first;
	first_free = cap = p.second;
}
