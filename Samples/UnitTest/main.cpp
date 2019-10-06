// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Core/Types/Char.hpp>
using namespace Sa;

int main()
{
	LOG("=== Start ===");


	LOG("\n=== _c8 ===");
	SA_TEST(120_c8, ==, 120);
	SA_TEST(-117_c8, ==, -117);
	SA_TEST(0x10FE52D9B864FA37_c8, ==, (char)0x37);


	LOG("\n=== _c16 ===");
	SA_TEST(120_c16, ==, 120u);
	SA_TEST(-117_c16, ==, -117);
	SA_TEST(0x10FE52D9B864FA37_c16, ==, 0xFA37u);


	LOG("\n=== _c32 ===");
	SA_TEST(120_c32, ==, 120u);
	SA_TEST(-117_c32, ==, -117u);
	SA_TEST(0x10FE52D9B864FA37_c32, ==, 0xB864FA37u);


	LOG("\n=== CharOfSize ===");
	SA_TEST(sizeof(CharOfSize<1>), ==, sizeof(char));
	SA_TEST(sizeof(CharOfSize<2>), ==, sizeof(char16_t));
	SA_TEST(sizeof(CharOfSize<4>), ==, sizeof(char32_t));


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
