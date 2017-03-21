#pragma once

#include <string>
#include <iostream>

#ifndef BIGNUMBER_H
#define BIGNUMBER_H

#define BASE 10
#define MAX(X, Y) ((X) > (Y)? (X) : (Y))

class bignumber
{
	friend std::ostream &operator<<(std::ostream &os, bignumber &bn);
	friend bignumber operator+(bignumber &a, bignumber &b);
	friend bignumber operator-(bignumber &a, bignumber &b);
	friend bignumber operator*(bignumber &a, bignumber &b);
	friend bignumber operator/(bignumber &a, bignumber &b);

public:
	bignumber(int len, char sign);
	bignumber(std::string &str);
	bignumber(const bignumber &bn);
	~bignumber();

	std::string to_string();
	int valid_len();

	bignumber &operator=(const bignumber &a);

private:
	char _sign;
	int _len;
	char *_data;

	void init(int len, char sign) { _sign = sign; _len = len; _data = new char[len](); }
	void copy_from(char * data) { for (size_t i = 0; i < _len; i++) _data[i] = data[i]; }
	void parse_bignumber(std::string &str);
	void fill_data_from_str(const std::string &str);
	void trim_str(std::string &str);
	void plus_one();

	static void add_impl(const bignumber & a, const bignumber & b, bignumber &r);
	static void sub_impl(const bignumber & a, const bignumber & b, bignumber &r);
	static void mul_impl(const bignumber & a, const bignumber & b, bignumber &r);

	static int cmp(bignumber & a, bignumber & b);
};

#endif // !BIGNUMBER_H
