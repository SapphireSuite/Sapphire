// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Core/Misc/Flags.hpp>
using namespace Sa;

enum class TestFlag : uint8
{
	Test0 = 0,
	Test1 = (1 << 1),
	Test2 = (1 << 2),
	Test3 = (1 << 3),
	Test4 = (1 << 4)
};
using TestFlags = Flags<TestFlag>;

int main()
{
	LOG("=== Start ===");


	TestFlags flag(TestFlag::Test1 | TestFlag::Test3);

	SA_TEST((uint8)flag, ==, (uint8)(TestFlag::Test1) | (uint8)(TestFlag::Test3));
	SA_TEST(flag.IsSet(TestFlag::Test1), ==, true);
	SA_TEST(flag.IsSet(TestFlag::Test2), == , false);


	LOG("\n=== Add ===");
	flag.Add(TestFlag::Test2);
	SA_TEST(flag.IsSet(TestFlag::Test2), == , true);


	LOG("\n=== Remove ===");
	flag.Remove(TestFlag::Test3);
	SA_TEST(flag.IsSet(TestFlag::Test3), == , false);


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
