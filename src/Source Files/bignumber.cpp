#include "bignumber.h"

bignumber::bignumber(int len, char sign) :
	_sign(sign),
	_len(len),
	_data(new char[len]())
{
}

bignumber::bignumber(std::string & str)
{
	init(str.size(), 0);
	parse_bignumber(str);
}

bignumber::~bignumber()
{
	delete[]_data;
}

void bignumber::parse_bignumber(std::string & str)
{
	trim_str(str);
	auto it = str.begin();

	if (*it == '-')
	{
		_sign = 1;
		it = str.erase(it);
		_len--;
	}
	else if (*it == '+')
	{
		_sign = 0;
		it = str.erase(it);
		_len--;
	}
	while (*it == '0' && str.size() > 1)
	{
		it = str.erase(it);
		_len--;
	}

	fill_data_from_str(str);
}

void bignumber::fill_data_from_str(const std::string & str)
{
	int length = str.size();

	for (size_t i = 0; i < length; i++)
	{
		char d = str[length - i - 1] - '0';

		if (d >= 0 && d <= 9)
		{
			_data[i] = d;
		}
		else
		{
			std::cerr << "Invalid Number " + str << std::endl;
			exit(1);
		}
	}
}

void bignumber::trim_str(std::string & str)
{
	auto it = str.begin();

	while (it != str.end())
	{
		if (iswspace(*it))
		{
			it = str.erase(it);
			_len--;
		}
		else
			it++;
	}
}
