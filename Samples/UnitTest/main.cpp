// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Core/Types/Variadics/Limits.hpp>
using namespace Sa;

int main()
{
	LOG("=== Start ===");


	LOG("\n=== bool ===");
	SA_TEST(Limits<bool>::min, ==, 0);
	SA_TEST(Limits<bool>::max, ==, 1);


	LOG("\n=== char8 ===");
	SA_TEST(Limits<char8>::min, ==, -128);
	SA_TEST(Limits<char8>::max, ==, 127);


	LOG("\n=== char16 ===");
	SA_TEST(Limits<char16>::min, ==, 0u);
	SA_TEST(Limits<char16>::max, ==, 65535u);


	LOG("\n=== char32 ===");
	SA_TEST(Limits<char32>::min, ==, 0u);
	SA_TEST(Limits<char32>::max, ==, 4294967295u);


	LOG("\n=== int8 / uint8 ===");
	SA_TEST(Limits<int8>::min, ==, -128);
	SA_TEST(Limits<int8>::max, ==, 127);
	SA_TEST(Limits<uint8>::min, ==, 0u);
	SA_TEST(Limits<uint8>::max, ==, 255u);


	LOG("\n=== int16 / uint16 ===");
	SA_TEST(Limits<int16>::min, ==, -32768);
	SA_TEST(Limits<int16>::max, ==, 32767);
	SA_TEST(Limits<uint16>::min, ==, 0u);
	SA_TEST(Limits<uint16>::max, ==, 65535u);


	LOG("\n=== int32 / uint32 ===");
	SA_TEST(Limits<int32>::min, ==, -2147483648);
	SA_TEST(Limits<int32>::max, ==, 2147483647);
	SA_TEST(Limits<uint32>::min, ==, 0u);
	SA_TEST(Limits<uint32>::max, ==, 4294967295u);


	LOG("\n=== int64 / uint64 ===");
	SA_TEST(Limits<int64>::min, ==, -9223372036854775807LL - 1);
	SA_TEST(Limits<int64>::max, ==, 9223372036854775807LL);
	SA_TEST(Limits<uint64>::min, ==, 0LLU);
	SA_TEST(Limits<uint64>::max, ==, 18446744073709551615LLU);


	LOG("\n=== float ===");
	SA_TEST(Limits<float>::min, ==, -FLT_MAX);
	SA_TEST(Limits<float>::max, ==, FLT_MAX);
	SA_TEST(Limits<float>::epsilon, ==, FLT_EPSILON);


	LOG("\n=== double ===");
	SA_TEST(Limits<double>::min, ==, -DBL_MAX);
	SA_TEST(Limits<double>::max, ==, DBL_MAX);
	SA_TEST(Limits<double>::epsilon, ==, DBL_EPSILON);


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
