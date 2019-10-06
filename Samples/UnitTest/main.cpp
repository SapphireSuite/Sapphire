// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Core/Misc/Random.hpp>
using namespace Sa;

static constexpr uint32 testNum = 10u;

int main()
{
	LOG("=== Start ===");


	LOG("\n=== Non-Seeded bool ===");
	std::cout << '{';
	for (uint32 i = 0u; i < testNum; i++)
	{
		std::cout << Random<bool>::Value();

		if(i + 1 < testNum)
			std::cout << ',';
	}
	LOG('}');


	const float ns_min = 10.0f;
	const float ns_max = 70.0f;
	LOG("\n=== Non-Seeded in range [" << ns_min << ',' << ns_max << "[ ===");

	for (uint32 i = 0u; i < testNum; i++)
	{
		const float rand = Random<float>::Value(ns_min, ns_max);
		LOG("--- Rand: " << rand << " ---");

		SA_TEST(rand, >=, ns_min);
		SA_TEST(rand, <, ns_max);
	}


	const int32 s_min = -43;
	const int32 s_max = 126;
	constexpr uint32 s_seed = 7u;
	LOG("\n=== Seeded with seed [" << s_seed << "] in range [" << s_min << ',' << s_max << "[ ===");

	RandEngine s_en1(s_seed);
	RandEngine s_en2(s_seed);

	for (uint32 i = 0u; i < testNum; i++)
	{
		const int32 rand1 = Random<int32>::Value(s_min, s_max, &s_en1);
		const int32 rand2 = Random<int32>::Value(s_min, s_max, &s_en2);

		LOG("--- Rand: " << rand1 << " ---");

		SA_TEST(rand1, >= , s_min);
		SA_TEST(rand1, < , s_max);
		SA_TEST(rand1, == , rand2);
	}


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
