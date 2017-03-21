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

bignumber::bignumber(const bignumber & bn)
{
	init(bn._len, bn._sign);
	copy_from(bn._data);
}

bignumber::~bignumber()
{
	delete []_data;
}

std::string bignumber::to_string()
{
	std::string str = std::string(_len, 0);

	for (size_t i = 0; i < _len; i++)
	{
		str[i] = _data[_len - i - 1] + '0';
	}
	trim_str(str);
	if (_sign == 1)
		str.insert(str.begin(), '-');

	return str;
}

int bignumber::valid_len()
{
	int len = _len;

	for (int i = len - 1; i >= 0; i--)
	{
		if (_data[i] == 0)
			len--;
		else
			break;
	}

	return len;
}

bignumber &bignumber::operator=(const bignumber & a)
{
	init(a._len, a._sign);
	copy_from(a._data);
	return *this;
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

	if (*it == '0')
		_sign = 0;

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

	while (it != str.end() && (iswspace(*it) || (*it == '0' && str.size() > 1)))
	{
		it = str.erase(it);
		_len--;
	}
}

void bignumber::plus_one()
{
	int length = _len;
	char carry = 1;

	for (size_t i = 0; i < length; i++)
	{
		carry += _data[i];
		_data[i] = carry % BASE;
		carry /= BASE;
	}
}

void bignumber::add_impl(const bignumber & a, const bignumber & b, bignumber &r)
{
	int length = r._len;
	int carry = 0;

	for (size_t i = 0; i < length; i++)
	{
		if (i < a._len) carry += a._data[i];
		if (i < b._len) carry += b._data[i];

		r._data[i] = carry % BASE;
		carry /= BASE;
	}
}

void bignumber::sub_impl(const bignumber & a, const bignumber & b, bignumber & r)
{
	int borrow = 0;
	int length = r._len;
	char tmp = 0;

	for (size_t i = 0; i < length; i++)
	{
		tmp = a._data[i] + BASE - borrow - ((i < b._len) ? b._data[i] : 0);
		r._data[i] = tmp % BASE;
		borrow = 1 - tmp / BASE;
	}
}

void bignumber::mul_impl(const bignumber & a, const bignumber & b, bignumber & r)
{
	int alen = a._len;
	int blen = b._len;
	char carry = 0;

	for (size_t i = 0; i < blen; i++)
	{
		size_t j = 0;
		for (j = 0; j < alen; j++)
		{
			carry += b._data[i] * a._data[j] + r._data[i + j];
			r._data[i + j] = carry % BASE;
			carry /= BASE;
		}

		for (; j + i < alen + blen; j++)
		{
			r._data[i + j] = carry % BASE;
			carry /= BASE;
		}
	}
}


int bignumber::cmp(bignumber & a, bignumber & b)
{
	if (a._sign == 0 && b._sign == 1)
		return 1;
	if (a._sign == 1 && b._sign == 0)
		return -1;

	int sign = a._sign;
	int alen = a.valid_len();
	int blen = b.valid_len();

	if (alen > blen)
		return (sign == 1 ? -1 : 1);
	else if (alen < blen)
		return (sign == 1 ? 1 : -1);
	else
	{
		for (int i = alen - 1; i >= 0; i--)
		{
			if (a._data[i] > b._data[i])
				return (sign == 1 ? -1 : 1);
			else if (a._data[i] < b._data[i])
				return (sign == 1 ? 1 : -1);
		}

		return 0;
	}
}

std::ostream &operator<<(std::ostream &os, bignumber &bn)
{
	int length = bn._len;
	std::string str = std::string(length, 0);

	for (size_t i = 0; i < length; i++)
	{
		str[i] = bn._data[length - i - 1] + '0';
	}
	bn.trim_str(str);
	if (bn._sign == 1)
		str.insert(str.begin(), '-');

	os << str;

	return os;
}

bignumber operator+(bignumber & a, bignumber & b)
{
	int len = MAX(a._len, b._len) + 1;
#if 1
	bignumber ret(len, 0);

	if (a._sign == b._sign)
	{
		ret._sign = a._sign;
		bignumber::add_impl(a, b, ret);
	}
	else if (a._sign == 0 && b._sign == 1)
	{
		b._sign = 0;
		ret = a - b;
	}
	else if (a._sign == 1 && b._sign == 0)
	{
		a._sign = 0;
		ret = b - a;
	}
#endif
	return ret;
}

bignumber operator-(bignumber & a, bignumber & b)
{
	int len = MAX(a._len, b._len);
	bignumber ret(len, 0);
#if 1
	if (a._sign == 0 && b._sign == 0)
	{
		if (bignumber::cmp(a, b) >= 0)
		{
			bignumber::sub_impl(a, b, ret);
		}
		else
		{
			ret._sign = 1;
			bignumber::sub_impl(b, a, ret);
		}
	}
	else if (a._sign == 1 && b._sign == 1)
	{
		a._sign = 0;
		b._sign = 0;

		ret = b - a;
	}
	else if (a._sign == 0 && b._sign == 1)
	{
		b._sign = 0;
		ret = a + b;
	}
	else if (a._sign == 1 && b._sign == 0)
	{
		a._sign = 0;
		ret = a + b;
		ret._sign = 1;
	}

#endif
	return ret;
}

bignumber operator*(bignumber & a, bignumber & b)
{
	int len = a._len + b._len;
	bignumber ret(len, a._sign == b._sign ? 0 : 1);

	bignumber::mul_impl(a, b, ret);

	return ret;
}

bignumber operator/(bignumber & a, bignumber & b)
{
	bignumber zero(1, 0);
	bignumber one(std::string("1"));
	bignumber two(std::string("2"));

	if (bignumber::cmp(b, zero) == 0)
	{
		std::cerr << "Integer division by zero" << std::endl;
	}
	else if (bignumber::cmp(a, zero) == 0)
		return zero;

	int len = a._len;
	bignumber ret_front(a._sign == b._sign ? std::string("1") : std::string("-1"));
	bignumber ret_later(a._sign == b._sign ? std::string("1") : std::string("-1"));
	bignumber tmp(len + 1, 0);
	a._sign = 0;
	b._sign = 0;

	while (true)
	{
		int cmp_ret = bignumber::cmp(a, tmp);

		if (cmp_ret > 0)
		{
			ret_front = ret_front * two;
			if (bignumber::cmp(tmp, zero) != 0)
				ret_later = ret_later * two;
			tmp = b * ret_front;
			tmp._sign = 0;
		}
		else if (cmp_ret == 0)
		{
			return ret_front;
		}
		else if (cmp_ret < 0)
		{
			tmp = ret_later * b;
			tmp._sign = 0;
			while (bignumber::cmp(a, tmp) > 0)
			{
				ret_later.plus_one();
				tmp = b * ret_later;
				tmp._sign = 0;
			}

			return ret_later;
		}

	}
}
