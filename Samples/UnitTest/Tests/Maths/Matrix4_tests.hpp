// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_TESTS_MATRIX4_GUARD
#define SAPPHIRE_TESTS_MATRIX4_GUARD

#include "../../UnitTest.hpp"

#include <Sapphire/Core/Misc/Random.hpp>
#include <Sapphire/Maths/Space/Matrix4.hpp>

#define LOG_V(_q) LOG(#_q ": " << _q)

namespace Sa
{
	std::ostream& operator<<(std::ostream& _stream, const Mat4d& _mat)
	{
		//_stream << '(' << _q.w << ',' << _q.x << ',' << _q.y << ',' << _q.z << ')';

		return _stream;
	}

	Mat4d GenerateRandMat()
	{
		return Mat4d(Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0));
	}

	void Test()
	{
		std::cout.precision(7);

		LOG("\n=== Constructors ===");
		{
			GenerateRandMat();
		}


		LOG("\n=== Equal ===");
		{

		}
	}
}

#endif // GUARD
