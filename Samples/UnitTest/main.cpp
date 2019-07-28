// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Core/Thread/Thread.hpp>
using namespace Sa;

int main()
{
	LOG("=== Start ===");


	uint32 count = 0u;
	bool bWaitForStart = true;

	Thread th1([&bWaitForStart, &count]()
	{
		while(bWaitForStart){}

		for (uint32 i = 0u; i < 5u; ++i)
		{
			++count;
			LOG("Th1");
		}
	});

	Thread th2([&bWaitForStart, &count]()
	{
		while(bWaitForStart){}

		for (uint32 i = 0u; i < 5u; ++i)
		{
			++count;
			LOG("Th2");
		}
	});

	LOG("Th1 ID: " << th1.GetID());
	LOG("Th2 ID: " << th2.GetID());

	SA_TEST(th1.GetID(), != , 0u);
	SA_TEST(th2.GetID(), !=, 0u);

	bWaitForStart = false;

	th1.Join();
	th2.Join();

	SA_TRY_TEST(count, ==, 10u);


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
