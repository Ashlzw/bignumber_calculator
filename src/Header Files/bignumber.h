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
	bignumber() : _sign(0), _len(0), _data() {}
	bignumber(const bignumber &bn);
	bignumber(std::string &str);
	bignumber(const char *str) : bignumber(std::string(str)) {}
	~bignumber();

	bignumber &operator=(const bignumber &a);

	void set_sign(int sign) { _sign = sign; }

	int length() { return _len; }
	std::string to_string();
	
private:
	char _sign;
	int _len;
	bignumber_data _data;

	int valid_len();
	
	void init(int len, char sign, const bignumber_data &dt) { _sign = sign; _len = len; _data = dt; }
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
