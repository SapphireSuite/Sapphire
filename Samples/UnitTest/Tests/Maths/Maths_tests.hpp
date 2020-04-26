// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_TESTS_MATHS_GUARD
#define SAPPHIRE_TESTS_MATHS_GUARD

#include "../../UnitTest.hpp"

#include <Sapphire/Core/Misc/Random.hpp>
#include <Sapphire/Maths/Misc/Degree.hpp>
#include <Sapphire/Maths/Misc/Radian.hpp>

namespace Sa
{
	void Test()
	{
		std::cout.precision(10);

		LOG("\n=== Equals ===");
		SA_TEST(Maths::Equals0(0.0), == , true);
		SA_TEST(Maths::Equals1(Random<double>::Value(0.1, 10.0)), == , false);

		double eq1 = Random<double>::Value(0.0, 1.0);
		double eq2 = eq1 + 2.0f * Limits<double>::epsilon;
		LOG("Rand: eq1 [" << eq1 << "] -- eq2 [" << eq2 << ']');

		SA_TEST(Maths::Equals(eq1, eq1), == , true);
		SA_TEST(Maths::Equals(eq1, eq2), == , false);
		SA_TEST(Maths::Equals(eq1, eq2, 3.0f * Limits<double>::epsilon), == , true);


		LOG("\n=== Abs ===");
		double abs1 = Random<double>::Value(0.0, 10.0); LOG("Rand: " << abs1);
		SA_TEST(Maths::Abs(-abs1), == , abs1);
		SA_TEST(Maths::Abs(abs1), == , abs1);


		LOG("\n=== Sign ===");
		double sign1 = Random<double>::Value(0.0, 10.0); LOG("Rand: " << sign1);
		SA_TEST(Maths::Sign(-sign1), == , -1.0f);
		SA_TEST(Maths::Sign(sign1), == , 1.0f);


		LOG("\n=== Mod ===");
		for (uint32 i = 0u; i < UnitTest::TestNum; ++i)
		{
			const int32 input = Random<int32>::Value(-10, 10);
			int32 mod = Random<int32>::Value(-10, 10);
			mod = mod == 0 ? 1 : mod; // avoid rand 0.

			LOG("Input: " << input << " - Mod: " << mod);
			SA_TEST(Maths::Mod(input, mod), == , (input % mod));
		}

		SA_TEST(Maths::Mod(3.456, 2.0), == , 1.456);
		SA_TEST(Maths::Mod(-3.456, 2.0), == , -1.456);
		SA_TEST(Maths::Mod(3.456, -2.0), == , 1.456);
		SA_TEST(Maths::Mod(-3.456, -2.0), == , -1.456);


		LOG("\n=== Min / Max / Clamp ===");
		for (uint32 i = 0u; i < UnitTest::TestNum; ++i)
		{
			const int32 min = Random<int32>::Value(-100, 100);
			const int32 max = Random<int32>::Value(min, 100);
			const int32 curr = Random<int32>::Value(-200, 200);

			LOG("Curr: " << curr << " -- Min: " << min << " -- Max: " << max);
			SA_TEST(Maths::Min(curr, min), == , (curr < min ? curr : min));
			SA_TEST(Maths::Max(curr, max), == , (curr > max ? curr : max));
			SA_TEST(Maths::Clamp(curr, min, max), == , (curr < min ? min : curr > max ? max : curr));
		}

		LOG("\n=== Lerp ===");
		SA_TEST(Maths::Lerp(10.0, 20.0, 0.25f), == , 12.5);
		SA_TEST(Maths::Lerp(10.0, 20.0, 0.5f), == , 15.0);
		SA_TEST(Maths::Lerp(10.0, 20.0, -1.0f), == , 10.0);
		SA_TEST(Maths::Lerp(10.0, 20.0, 2.0f), == , 20.0);

		SA_TEST(Maths::LerpUnclamped(10.0, 20.0, -1.0f), == , 0.0);
		SA_TEST(Maths::LerpUnclamped(10.0, 20.0, 2.0f), == , 30.0);


		LOG("\n=== Sqrt ===");
		SA_TEST(Maths::Sqrt(4.0), == , 2.0);
		SA_TEST(Maths::Sqrt(25.0), == , 5.0);


		LOG("\n=== Pow ===");
		SA_TEST(Maths::Pow(2.0, 3.0), == , 8.0);
		SA_TEST(Maths::Pow(4.0, 6.0), == , 4096.0);


		LOG("\n=== Degree ===");
		SA_TEST(Degd(45.0), == , 45.0);
		SA_TEST(45.0_deg, == , 45.0);
		SA_TEST(Degd(Radd(Maths::Pi)), == , 180.0);
		SA_TEST(Degd(Radd(Maths::PiOv2)), == , 90.0);


		LOG("\n=== Radian ===");
		SA_TEST(Radd(2.4), == , 2.4);
		SA_TEST(2.4_rad, == , 2.4);
		SA_TEST(Radd(Degd(180.0)), == , Maths::Pi);
		SA_TEST(Radd(Degd(90.0)), == , Maths::PiOv2);
	}
}

#endif // GUARD
