// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Core/Thread/Atomic.hpp>
#include <Sapphire/Core/Thread/Thread.hpp>
using namespace Sa;

int main()
{
	LOG("=== Start ===");


	Atomic<bool> waitForOther = true;

	uint32 count = 0u;

	Thread th1([&waitForOther, &count]()
	{
		while (waitForOther)
			Thread::Yield();

		waitForOther = true;

		for (uint32 i = 0u; i < 5u; ++i)
		{
			++count;
			LOG("Th1");
		}

		waitForOther = false;
	});

	Thread th2([&waitForOther, &count]()
	{
		while (waitForOther)
			Thread::Yield();

		waitForOther = true;

		for (uint32 i = 0u; i < 5u; ++i)
		{
			++count;
			LOG("Th2");
		}

		waitForOther = false;
	});

	LOG("Th1 ID: " << th1.GetID());
	LOG("Th2 ID: " << th2.GetID());

	SA_TEST(th1.GetID(), != , 0u);
	SA_TEST(th2.GetID(), !=, 0u);

	waitForOther = false;

	th1.Join();
	th2.Join();

	SA_TEST(count, ==, 10u);


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
