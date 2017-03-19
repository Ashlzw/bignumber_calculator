#pragma once

#include <string>
#include <iostream>

#ifndef BIGNUMBER_H
#define BIGNUMBER_H

class bignumber
{
	friend std::ostream &operator<<(std::ostream &os, bignumber &bn)
	{
		int length = bn._len;
		std::string str = std::string(length, 0);

		for (size_t i = 0; i < length; i++)
		{
			str[i] = bn._data[length - i - 1] + '0';
		}
		bn.trim_str(str);
		if (bn._sign == 1 && bn._len > 1)
			str.insert(str.begin(), '-');

		os << str;

		return os;
	}

public:
	char _sign;
	int _len;
	char *_data;

	bignumber(int len, char sign);
	bignumber(std::string &str);
	~bignumber();

	void parse_bignumber(std::string &str);
	void fill_data_from_str(const std::string &str);
	void trim_str(std::string &str);

private:
	void init(int len, char sign) { _sign = sign; _len = len; _data = new char[len](); }

};

#endif // !BIGNUMBER_H
