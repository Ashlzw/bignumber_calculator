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
	friend bignumber operator+(const bignumber &a, const bignumber &b);
	friend bignumber operator-(const bignumber &a, const bignumber &b);
	friend bignumber operator*(const bignumber &a, const bignumber &b);
	friend bignumber operator/(const bignumber &a, const bignumber &b);
public:
	bignumber(int len, char sign);
	bignumber(std::string &str);
	bignumber(const bignumber &bn);
	~bignumber();

	bignumber &operator=(const bignumber &a);

	void set_sign(int sign) { _sign = sign; }

	int length() { return _len; }
	std::string to_string();
	
private:
	char _sign;
	int _len;
	char *_data;

	int valid_len();
	
	void init(int len, char sign) { _sign = sign; _len = len; _data = new char[len] {0}; }
	void copy_from(char * data) { for (size_t i = 0; i < _len; i++) _data[i] = data[i]; }
	void parse_bignumber(std::string &str);
	void fill_data_from_str(const std::string &str);
	
	static void trim_str(std::string &str);
	static void add_impl(const bignumber & a, const bignumber & b, bignumber &r);
	static void sub_impl(const bignumber & a, const bignumber & b, bignumber &r);
	static void mul_impl(const bignumber & a, const bignumber & b, bignumber &r);
	
	static int cmp(const bignumber & a, const bignumber & b);
	static int no_sign_cmp(const bignumber & a, const bignumber & b);
};

#endif // !BIGNUMBER_H
