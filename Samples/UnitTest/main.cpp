// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Core/Misc/RAII.hpp>
using namespace Sa;

struct A
{
	A()
	{
		LOG("Construct A");
	}
	~A()
	{
		LOG("Destruct A");
	}
};

template <>
class Sa::RAII<A> final : RAIIBase
{
	A mHandle;
public:
};


int main()
{
	LOG("=== Start ===");


	bool bTest = false;

	{
		RAII<A> temp;

		RAII<bool> temp1(bTest);
		SA_TEST(bTest, ==, true)
	}

	SA_TEST(bTest, == , false)


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
