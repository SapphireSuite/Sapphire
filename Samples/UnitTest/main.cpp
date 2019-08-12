// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Core/Types/Int.hpp>
using namespace Sa;

int main()
{
	LOG("=== Start ===");


	LOG("\n=== _i8 / _ui8 ===");
	SA_TEST(120_i8, ==, 120);
	SA_TEST(120_ui8, ==, 120u);
	SA_TEST(-117_i8, ==, -117);
	SA_TEST(0x10FE52D9B864FA37_ui8, ==, (char)0x37);


	LOG("\n=== _i16 / _ui16 ===");
	SA_TEST(120_i16, ==, 120);
	SA_TEST(120_ui16, ==, 120u);
	SA_TEST(-117_i16, ==, -117);
	SA_TEST(0x10FE52D9B864FA37_ui16, ==, 0xFA37u);


	LOG("\n=== _i32 / _ui32 ===");
	SA_TEST(120_i32, ==, 120);
	SA_TEST(120_ui32, ==, 120u);
	SA_TEST(-117_i32, ==, -117);
	SA_TEST(0x10FE52D9B864FA37_ui32, ==, 0xB864FA37u);

	LOG("\n=== _i64 / _ui64 ===");
	SA_TEST(120_i64, ==, 120);
	SA_TEST(120_ui64, ==, 120u);
	SA_TEST(-117_i64, ==, -117);
	SA_TEST(0x10FE52D9B864FA37_ui64, ==, 0x10FE52D9B864FA37LLU);


	LOG("\n=== IntOfSize ===");
	SA_TEST(sizeof(IntOfSize<1>), ==, sizeof(char));
	SA_TEST(sizeof(IntOfSize<2>), ==, sizeof(short int));
	SA_TEST(sizeof(IntOfSize<4>), ==, sizeof(int));
	SA_TEST(sizeof(IntOfSize<8>), ==, sizeof(long long int));


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
