#pragma once
#ifndef BIGNUMBER_DATA_H
#define BIGNUMBER_DATA_H

#include <iostream>
#include <memory>

class bignumber_data
{
public:
	bignumber_data() : datas(0), first_free(0), cap(0) {}
	bignumber_data(const bignumber_data &bd);
	bignumber_data(const char *, const char *);
	bignumber_data &operator=(const bignumber_data bd);
	~bignumber_data() { free(); }

	void push_back(const char);

	size_t size() const { return first_free - datas; }
	size_t capacity() const { return cap - datas; };

	char *begin() const { return datas; };
	char *end() const { return first_free; };

	char &operator[](std::size_t n);
	const char &operator[](std::size_t n) const;
	
private:
	static std::allocator<char> alloc;

	void chk_n_alloc() { if (size() == capacity()) reallocate(); }
	void free();
	void reallocate();

	void swap(const bignumber_data &bd);

	std::pair<char *, char*> alloc_n_copy(const char*, const char*);

	char *datas;
	char *first_free;
	char *cap;
};

#endif // BIGNUMBER_DATA_H
