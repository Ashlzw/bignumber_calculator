#include "bignumber.h"

bignumber::bignumber(std::string & str)
{	
	parse_bignumber(str);
	_len = str.size();
	fill_data_from_str(str);
}

bignumber::bignumber(const bignumber & bn)
{
	init(bn._len, bn._sign, bn._data);
}

bignumber::~bignumber()
{
}

std::string bignumber::to_string()
{	
	std::string str;

	if (_sign == 1)
		str.push_back('-');
	for (size_t i = 0; i < _len; i++)
	{
		str.push_back(_data[_len - i - 1] + '0');
	}
	trim_str(str);

	return str;
}

int bignumber::valid_len()
{
	int len = _data.size();

	for (int i = len - 1; i >= 0; i--)
	{
		if (_data[i] == '0' || _data[i] == 0)
			len--;
		else
			break;
	}

	return !len ? 1 : len;
}

bignumber &bignumber::operator=(const bignumber & a)
{	
	init(a._len, a._sign, a._data);
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
	}
	else if (*it == '+')
	{
		_sign = 0;
		it = str.erase(it);
	}
	else
		_sign = 0;

	if (*it == '0')
		_sign = 0;
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
		it = str.erase(it);
}

void bignumber::add_impl(const bignumber & a, const bignumber & b, bignumber &r)
{
	int carry = 0;

	for (size_t i = 0; i < length; i++)
	{
		if (i < a._len) carry += a._data[i];
		if (i < b._len) carry += b._data[i];

		r._data.push_back(carry % BASE);
		carry /= BASE;
	}
	if (carry)
		r._data.push_back(carry);
}

void bignumber::sub_impl(const bignumber & a, const bignumber & b, bignumber & r)
{
	int borrow = 0;
	char tmp = 0;

	for (size_t i = 0; i < a._len; i++)
	{
		tmp = a._data[i] + BASE - borrow - ((i < b._len) ? b._data[i] : 0);
		r._data.push_back(tmp % BASE);
		borrow = 1 - tmp / BASE;
	}
}

void bignumber::mul_impl(const bignumber & a, const bignumber & b, bignumber & r)
{
	int alen = a._len;
	int blen = b._len;
	char *tmp = new char[alen + blen]{0};
	char carry = 0;

	for (size_t i = 0; i < blen; i++)
	{
		size_t j = 0;
		for (; j < alen; j++)
		{
			carry += b._data[i] * a._data[j] + tmp[i + j];
			tmp[i + j] = carry % BASE;
			carry /= BASE;
		}

		for (; j + i < alen + blen; j++)
		{
			tmp[i + j] = carry % BASE;
			carry /= BASE;
		}
	}
	size_t sz = alen + blen;
	for (int i = alen + blen - 1; i >= 0; --i)
		if (tmp[i] == 0)
			--sz;
		else
			break;
	sz = sz ? sz : 1;
	try
	{
		r._data = bignumber_data(tmp, tmp + sz);
	}
	catch (const std::exception&)
	{
		delete tmp;
	}
	delete tmp;
}


int bignumber::cmp(bignumber & a, bignumber & b)
{
	if (a._sign == 0 && b._sign == 1)
		return 1;
	if (a._sign == 1 && b._sign == 0)
		return -1;

	int sign = a._sign;
	int alen = a._len;
	int blen = b._len;

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

int bignumber::no_sign_cmp(const bignumber & a, const bignumber & b)
{
	int alen = a._len;
	int blen = b._len;
	if (alen > blen)
		return 1;
	else if (alen < blen)
		return -1;
	else
	{
		for (int i = alen - 1; i >= 0; i--)
		{
			if (a._data[i] > b._data[i])
				return 1;
			else if (a._data[i] < b._data[i])
				return -1;
		}

		return 0;
	}
}

std::ostream &operator<<(std::ostream &os, bignumber &bn)
{
	int length = bn._len;	

	std::string str;

	if (bn._sign == 1)
		str.push_back('-');
	for (size_t i = 0; i < length; i++)
	{
		str.push_back(bn._data[length - i - 1] + '0');
	}
	bignumber::trim_str(str);

	os << str;

	return os;
}

bignumber operator+(const bignumber & a, const bignumber & b)
{

#if 1
	bignumber ret;

	if (a._sign == b._sign)
	{
		ret._sign = a._sign;
		bignumber::add_impl(a, b, ret);
	}
	else if (a._sign == 0 && b._sign == 1)
	{		
		if (bignumber::no_sign_cmp(a, b) >= 0)
			bignumber::sub_impl(a, b, ret);
		else
		{
			bignumber::sub_impl(b, a, ret);
			ret.set_sign(1);
		}
	}
	else if (a._sign == 1 && b._sign == 0)
	{
		if (bignumber::no_sign_cmp(b, a) >= 0)
			bignumber::sub_impl(b, a, ret);
		else
		{
			bignumber::sub_impl(a, b, ret);
			ret.set_sign(1);
		}
	}
#endif
	ret._len = ret.valid_len();
	return ret;
}

bignumber operator-(const bignumber & a, const bignumber & b)
{
	bignumber ret;
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
		if (bignumber::no_sign_cmp(a, b) > 0)
		{
			ret._sign = 1;
			bignumber::sub_impl(a, b, ret);
		}
		else
		{
			bignumber::sub_impl(b, a, ret);
		}
	}
	else if (a._sign == 0 && b._sign == 1)
	{		
		bignumber::add_impl(a, b, ret);
	}
	else if (a._sign == 1 && b._sign == 0)
	{		
		bignumber::add_impl(a, b, ret);
		ret._sign = 1;
	}

#endif
	ret._len = ret.valid_len();
	return ret;
}

bignumber operator*(const bignumber & a, const bignumber & b)
{
	int len = a._len + b._len;
	bignumber ret;

	bignumber::mul_impl(a, b, ret);
	ret.set_sign(a._sign == b._sign ? 0 : 1);
	ret._len = ret.valid_len();

	return ret;
}

bignumber operator/(const bignumber & a, const bignumber & b)
{	
	bignumber ret("0");
	bignumber two("2");
	bignumber aTmp = a;
	bignumber bTmp = b;

	if (bignumber::cmp(bTmp, ret) == 0)
	{
		std::cerr << "Integer division by zero" << std::endl;
	}
	else if (bignumber::cmp(aTmp, ret) == 0)
		return ret;

	char asign = aTmp._sign, bsign = bTmp._sign;
	aTmp._sign = 0;
	bTmp._sign = 0;

	while (bignumber::cmp(aTmp, bTmp) >= 0)
	{
		bignumber multi("1");
		bignumber tmp = bTmp;
		while (bignumber::cmp(aTmp, (tmp * two)) >= 0)
		{
			tmp = tmp * two;
			multi = multi * two;
		}
		aTmp = aTmp - tmp;
		ret = ret + multi;
	}
	ret.set_sign(asign == bsign ? 0 : 1);
	ret._len = ret.valid_len();
	return ret;
}
