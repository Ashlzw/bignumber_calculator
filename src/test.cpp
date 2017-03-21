#include "bignumber.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

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
		EXPECT_EQ_INTEGER(expect, bnum.to_string());\
	} while (0)

#define TEST_OPERATOR(expect, input1, input2, op)\
	do\
	{\
		bignumber a(input1);\
		bignumber b(input2);\
		TEST_BIGNUMBER(expect, a op b);\
	} while (0)

#define TEST_ADD(expect, input1, input2) TEST_OPERATOR(expect, input1, input2, +)

#define TEST_SUB(expect, input1, input2) TEST_OPERATOR(expect, input1, input2, -)

#define TEST_MUL(expect, input1, input2) TEST_OPERATOR(expect, input1, input2, *)

#define TEST_DIV(expect, input1, input2) TEST_OPERATOR(expect, input1, input2, /)

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

static void test_bignumber_add()
{
	TEST_ADD(std::string("0"), std::string("0"), std::string("0"));
	TEST_ADD(std::string("0"), std::string("-0"), std::string("-0"));
	TEST_ADD(std::string("1"), std::string("0"), std::string("1"));
	TEST_ADD(std::string("-1"), std::string("0"), std::string("-1"));
	TEST_ADD(std::string("100"), std::string("99"), std::string("1"));
	TEST_ADD(std::string("99"), std::string("100"), std::string("-1"));
	TEST_ADD(std::string("1070553071"), std::string("85894615"), std::string("984658456"));
	TEST_ADD(std::string("85894614"), std::string("85894615"), std::string("-1"));
	TEST_ADD(std::string("22222222222222"), std::string("22222222222222"), std::string("0"));
	TEST_ADD(std::string("22222222222222"), std::string("11111111111111"), std::string("11111111111111"));
	TEST_ADD(std::string("-22222222222222"), std::string("-11111111111111"), std::string("-11111111111111"));
	TEST_ADD(std::string("11111111111111"), std::string("22222222222222"), std::string("-11111111111111"));
	TEST_ADD(std::string("10000000000000"), std::string("9999999999999"), std::string("1"));
	TEST_ADD(std::string("-10000000000000"), std::string("-9999999999999"), std::string("-1"));
}

static void test_bignumber_sub()
{
	TEST_SUB(std::string("0"), std::string("0"), std::string("0"));
	TEST_SUB(std::string("1"), std::string("1"), std::string("0"));
	TEST_SUB(std::string("0"), std::string("1"), std::string("1"));
	TEST_SUB(std::string("0"), std::string("-1"), std::string("-1"));
	TEST_SUB(std::string("100"), std::string("101"), std::string("1"));
	TEST_SUB(std::string("102"), std::string("101"), std::string("-1"));
	TEST_SUB(std::string("99"), std::string("100"), std::string("1"));
	TEST_SUB(std::string("-101"), std::string("-100"), std::string("1"));
	TEST_SUB(std::string("85894615"), std::string("1070553071"),  std::string("984658456"));
	TEST_SUB(std::string("-85894615"), std::string("-1070553071"), std::string("-984658456"));
	TEST_SUB(std::string("22222222222222"), std::string("22222222222222"), std::string("0"));
	TEST_SUB(std::string("-22222222222222"), std::string("-22222222222222"), std::string("0"));
	TEST_SUB(std::string("11111111111111"), std::string("22222222222222"), std::string("11111111111111"));
	TEST_SUB(std::string("-33333333333333"), std::string("-22222222222222"), std::string("11111111111111"));
	TEST_SUB(std::string("-10000000000000"), std::string("0"), std::string("10000000000000"));
}

static void test_bignumber_mul()
{
	TEST_MUL(std::string("0"), std::string("0"), std::string("0"));
	TEST_MUL(std::string("0"), std::string("1"), std::string("0"));
	TEST_MUL(std::string("1"), std::string("1"), std::string("1"));
	TEST_MUL(std::string("1"), std::string("-1"), std::string("-1"));
	TEST_MUL(std::string("-1"), std::string("1"), std::string("-1"));
	TEST_MUL(std::string("101"), std::string("101"), std::string("1"));
	TEST_MUL(std::string("-101"), std::string("101"), std::string("-1"));
	TEST_MUL(std::string("1000"), std::string("40"), std::string("25"));
	TEST_MUL(std::string("85284543725746406280"), std::string("98646549864"), std::string("864546645"));
	TEST_MUL(std::string("-85284543725746406280"), std::string("98646549864"), std::string("-864546645"));
	TEST_MUL(std::string("0"), std::string("22222222222222"), std::string("0"));
	TEST_MUL(std::string("-2222222222222222222222222222222222222222222222222222"), std::string("-2222222222222222222222222222222222222222222222222222"), std::string("1"));
	TEST_MUL(std::string("99999999999999"), std::string("9"), std::string("11111111111111"));
}

static void test_bignumber_div()
{
	TEST_DIV(std::string("1"), std::string("1"), std::string("1"));
	TEST_DIV(std::string("3"), std::string("3"), std::string("1"));
	TEST_DIV(std::string("33"), std::string("33"), std::string("1"));
	TEST_DIV(std::string("2049"), std::string("2049"), std::string("1"));
	TEST_DIV(std::string("1"), std::string("-1"), std::string("-1"));
	TEST_DIV(std::string("-1"), std::string("1"), std::string("-1"));
	TEST_DIV(std::string("101"), std::string("101"), std::string("1"));
	TEST_DIV(std::string("-101"), std::string("101"), std::string("-1"));
	TEST_DIV(std::string("40"), std::string("1000"), std::string("25"));
/*	TEST_DIV(std::string("98646549864"), std::string("85284543725746406280"), std::string("864546645"));
	TEST_DIV(std::string("98646549864"), std::string("-85284543725746406280"), std::string("-864546645"));
	TEST_DIV(std::string("-2222222222222222222222222222222222222222222222222222"), std::string("-2222222222222222222222222222222222222222222222222222"), std::string("1"));
	TEST_DIV(std::string("9"), std::string("99999999999999"), std::string("11111111111111"));
*/
}

int main()
{
	test_bignumber_construct();
	test_bignumber_add();
	test_bignumber_sub();
	test_bignumber_mul();
	test_bignumber_div();

	std::cout.precision(8);
	std::cout << test_pass << "/" << test_count << " (" << test_pass * 100.0 / test_count << "%) passed" << std::endl;

	return main_ret;
}
