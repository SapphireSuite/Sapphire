// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_TESTS_QUATERNION_GUARD
#define SAPPHIRE_TESTS_QUATERNION_GUARD

#include "../../UnitTest.hpp"

#include <Sapphire/Core/Misc/Random.hpp>
#include <Sapphire/Maths/Space/Vector3.hpp>
#include <Sapphire/Maths/Space/Quaternion.hpp>

#define LOG_V(_q) LOG(#_q ": " << _q)

namespace Sa
{
	std::ostream& operator<<(std::ostream& _stream, const Quatd& _q)
	{
		_stream << '(' << _q.w << ',' << _q.x << ',' << _q.y << ',' << _q.z << ')';

		return _stream;
	}

	Quatd GenerateRandQuaternion()
	{
		return Quatd(Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0)).Normalize();
	}

	Vec3d GenerateRandUnitVector()
	{
		return Vec3d(Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0),
			Random<double>::Value(-100.0, 100.0)).Normalize();
	}

	void Test()
	{
		std::cout.precision(7);

		LOG("\n=== Constructors ===");
		{
			const double constr_q1W = Random<double>::Value(-100.0, 100.0);
			const double constr_q1X = Random<double>::Value(-100.0, 100.0);
			const double constr_q1Y = Random<double>::Value(-100.0, 100.0);
			const double constr_q1Z = Random<double>::Value(-100.0, 100.0);
			const Quatd constr_q1(constr_q1W, constr_q1X, constr_q1Y, constr_q1Z); LOG_V(constr_q1);

			SA_TEST(constr_q1.w, == , constr_q1W);
			SA_TEST(constr_q1.x, == , constr_q1X);
			SA_TEST(constr_q1.y, == , constr_q1Y);
			SA_TEST(constr_q1.z, == , constr_q1Z);


			const Quatd constr_q2(20.0_deg, Vec3d(0.75f, 1.0f, 0.0f).Normalize()); LOG_V(constr_q2);
			SA_TEST(Maths::Equals(constr_q2.w, 0.9848078, 0.0000001), ==, true);
			SA_TEST(Maths::Equals(constr_q2.x, 0.1041889, 0.0000001), ==, true);
			SA_TEST(Maths::Equals(constr_q2.y, 0.1389185, 0.0000001), ==, true);
			SA_TEST(Maths::Equals(constr_q2.z, 0.0), ==, true);


			const float constr_q3W = Random<float>::Value(-100, 100);
			const float constr_q3X = Random<float>::Value(-100, 100);
			const float constr_q3Y = Random<float>::Value(-100, 100);
			const float constr_q3Z = Random<float>::Value(-100, 100);
			const Quatd constr_q3(Quatf(constr_q3W, constr_q3X, constr_q3Y, constr_q3Z)); LOG_V(constr_q3);

			SA_TEST(constr_q3.w, == , static_cast<double>(constr_q3W));
			SA_TEST(constr_q3.x, == , static_cast<double>(constr_q3X));
			SA_TEST(constr_q3.y, == , static_cast<double>(constr_q3Y));
			SA_TEST(constr_q3.z, == , static_cast<double>(constr_q3Z));


			const Quatd constr_q4(constr_q1); LOG_V(constr_q4);

			SA_TEST(constr_q4.w, == , constr_q1.w);
			SA_TEST(constr_q4.x, == , constr_q1.x);
			SA_TEST(constr_q4.y, == , constr_q1.y);
			SA_TEST(constr_q4.z, == , constr_q1.z);
		}


		LOG("\n=== Equal ===");
		{
			const Quatd eq_q1 = GenerateRandQuaternion(); LOG_V(eq_q1);
			const Quatd eq_q2 = GenerateRandQuaternion(); LOG_V(eq_q2);

			SA_TEST(eq_q1.IsZero(), == , false);
			SA_TEST(Quatd::Zero.IsZero(), == , true);

			SA_TEST(eq_q1.Equals(eq_q1), == , true);
			SA_TEST(eq_q1.Equals(eq_q2), == , false);

			SA_TEST(eq_q1, == , eq_q1);
			SA_TEST(eq_q1, != , eq_q2);
		}


		LOG("\n=== Length ===");
		for (uint32 i = 0u; i < UnitTest::TestNum; ++i)
		{
			Quatd len_q = Quatd(Random<double>::Value(-100.0, 100.0),
				Random<double>::Value(-100.0, 100.0),
				Random<double>::Value(-100.0, 100.0),
				Random<double>::Value(-100.0, 100.0)); LOG_V(len_q);

			const double len2_qd = len_q.w * len_q.w + len_q.x * len_q.x + len_q.y * len_q.y + len_q.z * len_q.z;
			const double len_qd = Maths::Sqrt(len2_qd);
			SA_TEST(len_q.Length(), == , len_qd);
			SA_TEST(len_q.SqrLength(), == , len2_qd);

			const Quatd nLen_q = len_q.GetNormalized(); LOG_V(nLen_q);
			SA_TEST(nLen_q.w, == , len_q.w / len_qd);
			SA_TEST(nLen_q.x, == , len_q.x / len_qd);
			SA_TEST(nLen_q.y, == , len_q.y / len_qd);
			SA_TEST(nLen_q.z, == , len_q.z / len_qd);

			SA_TEST(nLen_q.IsNormalized(), == , true);
			SA_TEST(len_q.IsNormalized(), == , false);

			len_q.Normalize();
			SA_TEST(len_q.w, == , nLen_q.w);
			SA_TEST(len_q.x, == , nLen_q.x);
			SA_TEST(len_q.y, == , nLen_q.y);
			SA_TEST(len_q.z, == , nLen_q.z);
		}


		LOG("\n=== Inverse ===");
		{
			Quatd inv_q1 = GenerateRandQuaternion();
			const Quatd inv_q2 = inv_q1.GetInversed();

			SA_TEST(inv_q2.w, == , inv_q1.w);
			SA_TEST(inv_q2.x, == , -inv_q1.x);
			SA_TEST(inv_q2.y, == , -inv_q1.y);
			SA_TEST(inv_q2.z, == , -inv_q1.z);

			inv_q1.Inverse();
			SA_TEST(inv_q1.Equals(inv_q2), == , true);
		}


		LOG("\n=== Dot ===");
		for (uint32 i = 0u; i < UnitTest::TestNum; ++i)
		{
			const Quatd dc_q1 = GenerateRandQuaternion(); LOG_V(dc_q1);
			const Quatd dc_q2 = GenerateRandQuaternion(); LOG_V(dc_q2);

			SA_TEST(Quatd::Dot(dc_q1, dc_q2), == , (dc_q1.w * dc_q2.w + dc_q1.x * dc_q2.x + dc_q1.y * dc_q2.y + dc_q1.z * dc_q2.z));
		}


		LOG("\n=== Angle ===");
		for (uint32 i = 0u; i < UnitTest::TestNum; ++i)
		{
			const Vec3d axis = GenerateRandUnitVector();
			const double angle = Random<double>::Value(0.0f, 360.0f);

			const Quatd angl_q1 = Quatd(angle, axis);

			SA_TEST(Maths::Equals(static_cast<double>(angl_q1.GetAngle()), angle, 0.0000001), == , true);

			const Vec3d outAxis = angl_q1.GetAxis();
			SA_TEST(Maths::Equals(outAxis.x, axis.x, 0.0000001), ==, true);
			SA_TEST(Maths::Equals(outAxis.y, axis.y, 0.0000001), ==, true);
			SA_TEST(Maths::Equals(outAxis.z, axis.z, 0.0000001), ==, true);
		}


		//LOG("\n=== Dist ===");
		//{
		//	const Vec3d dist_q1 = GenerateRandQuaternion(); LOG_V(dist_q1);
		//	const Vec3d dist_q2 = GenerateRandQuaternion(); LOG_V(dist_q2);

		//	SA_TEST(Vec3d::Dist(dist_q1, dist_q2), == , (dist_q1 - dist_q2).Length());
		//	SA_TEST(Vec3d::SqrDist(dist_q1, dist_q2), == , (dist_q1 - dist_q2).SqrLength());
		//}


		//LOG("\n=== Dir ===");
		//for (uint32 i = 0u; i < UnitTest::TestNum; ++i)
		//{
		//	const Vec3d dir_q1 = GenerateRandQuaternion(); LOG_V(dir_q1);
		//	const Vec3d dir_q2 = GenerateRandQuaternion(); LOG_V(dir_q2);

		//	const double dirX = dir_q2.x - dir_q1.x;
		//	const double dirY = dir_q2.y - dir_q1.y;
		//	const double dirZ = dir_q2.z - dir_q1.z;
		//	const double dirLen = Maths::Sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);

		//	SA_TEST(Vec3d::Dir(dir_q1, dir_q2).x, == , dirX);
		//	SA_TEST(Vec3d::Dir(dir_q1, dir_q2).y, == , dirY);
		//	SA_TEST(Vec3d::Dir(dir_q1, dir_q2).z, == , dirZ);

		//	SA_TEST(Vec3d::DirN(dir_q1, dir_q2).x, == , dirX / dirLen);
		//	SA_TEST(Vec3d::DirN(dir_q1, dir_q2).y, == , dirY / dirLen);
		//	SA_TEST(Vec3d::DirN(dir_q1, dir_q2).z, == , dirZ / dirLen);
		//}


		//LOG("\n=== Lerp / SLerp ===");
		//{
		//	const Vec3f lerp_q1(2.0f, 2.0f, 0.0f); LOG_V(lerp_q1);
		//	const Vec3f lerp_q2(-2.0f, 4.0f, 8.0f); LOG_V(lerp_q2);

		//	const Vec3f lerp_res05 = Vec3f::Lerp(lerp_q1, lerp_q2, 0.5f);
		//	SA_TEST(lerp_res05.x, == , 0.0f);
		//	SA_TEST(lerp_res05.y, == , 3.0f);
		//	SA_TEST(lerp_res05.z, == , 4.0f);

		//	const Vec3f lerp_res2 = Vec3f::Lerp(lerp_q1, lerp_q2, 2.0f);
		//	SA_TEST(lerp_res2.x, == , lerp_q2.x);
		//	SA_TEST(lerp_res2.y, == , lerp_q2.y);
		//	SA_TEST(lerp_res2.z, == , lerp_q2.z);

		//	const Vec3f ulerp_res1 = Vec3f::LerpUnclamped(lerp_q1, lerp_q2, -1.0f);
		//	SA_TEST(ulerp_res1.x, == , 6.0f);
		//	SA_TEST(ulerp_res1.y, == , 0.0f);
		//	SA_TEST(ulerp_res1.z, == , -8.0f);


		//	const Vec3f slerp_q1(2.0f, 2.0f, 0.0f); LOG_V(slerp_q1);
		//	const Vec3f slerp_q2(-2.0f, 2.0f, 0.0f); LOG_V(slerp_q2);
		//	const Vec3f slerp_res05 = Vec3f::SLerp(slerp_q1, slerp_q2, 0.5f);
		//	SA_TEST(slerp_res05.x, == , 0.0f);
		//	SA_TEST(slerp_res05.y, == , slerp_q1.Length());
		//	SA_TEST(slerp_res05.z, == , 0.0f);
		//}


		//LOG("\n=== Operator ===");
		//{
		//	const Vec3d op_q1 = GenerateRandQuaternion(); LOG_V(op_q1);

		//	SA_TEST(-op_q1.x, == , -op_q1.x);
		//	SA_TEST(-op_q1.y, == , -op_q1.y);
		//	SA_TEST(-op_q1.z, == , -op_q1.z);

		//	double op_scale = Random<double>::Value(-100.0, 100.0); LOG("Scale: " << op_scale);
		//	SA_TEST((op_q1 * op_scale).x, == , op_q1.x * op_scale);
		//	SA_TEST((op_q1 * op_scale).y, == , op_q1.y * op_scale);
		//	SA_TEST((op_q1 * op_scale).z, == , op_q1.z * op_scale);

		//	SA_TEST((op_scale * op_q1).x, == , op_q1.x * op_scale);
		//	SA_TEST((op_scale * op_q1).y, == , op_q1.y * op_scale);
		//	SA_TEST((op_scale * op_q1).z, == , op_q1.z * op_scale);

		//	SA_TEST((op_q1 / op_scale).x, == , op_q1.x / op_scale);
		//	SA_TEST((op_q1 / op_scale).y, == , op_q1.y / op_scale);
		//	SA_TEST((op_q1 / op_scale).z, == , op_q1.z / op_scale);

		//	SA_TEST((op_scale / op_q1).x, == , op_scale / op_q1.x);
		//	SA_TEST((op_scale / op_q1).y, == , op_scale / op_q1.y);
		//	SA_TEST((op_scale / op_q1).z, == , op_scale / op_q1.z);

		//	const Vec3d op_q2 = GenerateRandQuaternion(); LOG_V(op_q2);
		//	SA_TEST((op_q1 + op_q2).x, == , op_q1.x + op_q2.x);
		//	SA_TEST((op_q1 + op_q2).y, == , op_q1.y + op_q2.y);
		//	SA_TEST((op_q1 + op_q2).z, == , op_q1.z + op_q2.z);

		//	SA_TEST((op_q1 - op_q2).x, == , op_q1.x - op_q2.x);
		//	SA_TEST((op_q1 - op_q2).y, == , op_q1.y - op_q2.y);
		//	SA_TEST((op_q1 - op_q2).z, == , op_q1.z - op_q2.z);

		//	SA_TEST((op_q1 * op_q2).x, == , op_q1.x * op_q2.x);
		//	SA_TEST((op_q1 * op_q2).y, == , op_q1.y * op_q2.y);
		//	SA_TEST((op_q1 * op_q2).z, == , op_q1.z * op_q2.z);

		//	SA_TEST((op_q1 / op_q2).x, == , op_q1.x / op_q2.x);
		//	SA_TEST((op_q1 / op_q2).y, == , op_q1.y / op_q2.y);
		//	SA_TEST((op_q1 / op_q2).z, == , op_q1.z / op_q2.z);

		//	Vec3d op_q3 = op_q1;
		//	op_q3 *= op_scale;
		//	SA_TEST(op_q3.x, == , op_q1.x * op_scale);
		//	SA_TEST(op_q3.y, == , op_q1.y * op_scale);
		//	SA_TEST(op_q3.z, == , op_q1.z * op_scale);

		//	Vec3d op_q4 = op_q1;
		//	op_q4 /= op_scale;
		//	SA_TEST(op_q4.x, == , op_q1.x / op_scale);
		//	SA_TEST(op_q4.y, == , op_q1.y / op_scale);
		//	SA_TEST(op_q4.z, == , op_q1.z / op_scale);

		//	Vec3d op_q5 = op_q1;
		//	op_q5 += op_q2;
		//	SA_TEST(op_q5.x, == , op_q1.x + op_q2.x);
		//	SA_TEST(op_q5.y, == , op_q1.y + op_q2.y);
		//	SA_TEST(op_q5.z, == , op_q1.z + op_q2.z);

		//	Vec3d op_q6 = op_q1;
		//	op_q6 -= op_q2;
		//	SA_TEST(op_q6.x, == , op_q1.x - op_q2.x);
		//	SA_TEST(op_q6.y, == , op_q1.y - op_q2.y);
		//	SA_TEST(op_q6.z, == , op_q1.z - op_q2.z);

		//	Vec3d op_q7 = op_q1;
		//	op_q7 *= op_q2;
		//	SA_TEST(op_q7.x, == , op_q1.x * op_q2.x);
		//	SA_TEST(op_q7.y, == , op_q1.y * op_q2.y);
		//	SA_TEST(op_q7.z, == , op_q1.z * op_q2.z);

		//	Vec3d op_q8 = op_q1;
		//	op_q8 /= op_q2;
		//	SA_TEST(op_q8.x, == , op_q1.x / op_q2.x);
		//	SA_TEST(op_q8.y, == , op_q1.y / op_q2.y);
		//	SA_TEST(op_q8.z, == , op_q1.z / op_q2.z);
		//}


		LOG("\n=== Operator [] ===");
		{
			const Quat opacc_q1 = GenerateRandQuaternion(); LOG_V(opacc_q1);

			SA_TEST(opacc_q1[0], == , opacc_q1.w);
			SA_TEST(opacc_q1[1], == , opacc_q1.x);
			SA_TEST(opacc_q1[2], == , opacc_q1.y);
			SA_TEST(opacc_q1[3], == , opacc_q1.z);
		}
	}
}

#endif // GUARD
