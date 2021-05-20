// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_TESTS_MATRIX4_GUARD
#define SAPPHIRE_TESTS_MATRIX4_GUARD

#include "../../UnitTest.hpp"

#include <Sapphire/Core/Misc/Random.hpp>
#include <Sapphire/Maths/Space/Matrix3.hpp>

#define LOG_V(_q) LOG(#_q ": " << _q)

namespace Sa
{
	std::ostream& operator<<(std::ostream& _stream, const Mat3d& _mat)
	{
		_stream << "Mat3(\n";

		for (uint32 i = 0; i < 3u; ++i)
		{
			for (uint32 j = 0; j < 3u; ++j)
				_stream << _mat.At(i, j) << ", \t";

			_stream << '\n';
		}

		_stream << ')';

		return _stream;
	}

	Mat3d GenerateRandMat()
	{
		return Mat3d(Random<double>::Value(-100.0, 100.0),
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
			std::cout << GenerateRandMat() << std::endl;
		}


		LOG("\n=== Equal ===");
		{

		}

		LOG("\n=== Inverse ===");
		{
			Mat3d m = GenerateRandMat();
			std::cout << "Mat: " << m << std::endl;

			Mat3d invM = m.GetInversed();
			std::cout << "InvMat: " << invM << std::endl;

			Mat3d result = invM * m;
			std::cout << "Mult: " << result << std::endl;
		}
	}
}

#endif // GUARD
