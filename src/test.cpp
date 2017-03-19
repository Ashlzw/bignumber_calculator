#include "bignumber.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

std::string data_to_str(bignumber &bn)
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

	return str;
}

#define EXPECT_EQ_BASE(equality, expect, actual) \
	do\
	{\
		test_count++;\
		if (equality)\
			test_pass++;\
		else\
		{\
			std::cerr << __FILE__ << ":" << __LINE__ << " line: expect: " << expect << " actual: " << actual << std::endl;\
			main_ret = 1;\
		}\
	} while (0)

#define EXPECT_EQ_INTEGER(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual)

#define TEST_BIGNUMBER(expect, actual)\
	do\
	{\
		bignumber bnum(actual);\
		EXPECT_EQ_INTEGER(expect, data_to_str(bnum));\
	} while (0)

static void test_bignumber_construct()
{
	TEST_BIGNUMBER(std::string("0"), std::string("0"));
	TEST_BIGNUMBER(std::string("1000000000000000000000"), std::string("1000000000000000000000"));
	TEST_BIGNUMBER(std::string("0"), std::string("-0"));
	TEST_BIGNUMBER(std::string("0"), std::string("00"));
	TEST_BIGNUMBER(std::string("123"), std::string("00 123"));
	TEST_BIGNUMBER(std::string("123"), std::string(" 123"));
	TEST_BIGNUMBER(std::string("-123"), std::string("-123"));
}

int main()
{
	test_bignumber_construct();

	std::cout.precision(8);
	std::cout << test_pass << "/" << test_count << " (" << test_pass * 100.0 / test_count << "%) passed" << std::endl;

	return main_ret;
}
