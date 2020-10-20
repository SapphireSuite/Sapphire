// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Mesh/RawMesh.hpp>

namespace Sa
{
	RawMesh RawMesh::SquareMesh() noexcept
	{
		RawMesh mesh;

		mesh.vertices =
		{
			{ { -0.5f, -0.5f, 0.0f }, Vec3f::Forward, Vec3f::Zero, { 1.0f, 0.0f } },
			{ { 0.5f, -0.5f, 0.0f }, Vec3f::Forward, Vec3f::Zero, { 0.0f, 0.0f } },
			{ { 0.5f, 0.5f, 0.0f }, Vec3f::Forward, Vec3f::Zero, { 0.0f, 1.0f } },
			{ { -0.5f, 0.5f, 0.0f }, Vec3f::Forward, Vec3f::Zero, { 1.0f, 1.0f } },
		};

		mesh.indices =
		{
			0, 1, 2, 2, 3, 0
		};

		return mesh;
	}

	RawMesh RawMesh::CubeMesh() noexcept
	{
		RawMesh mesh;

		mesh.vertices =
		{
			// Front face.
			{ { -0.5f, -0.5f, 0.5f }, Vec3f::Forward, Vec3f::Zero, { 1.0f, 0.0f } },	// 0.
			{ { 0.5f, -0.5f, 0.5f }, Vec3f::Forward, Vec3f::Zero, { 0.0f, 0.0f } },		// 1.
			{ { 0.5f, 0.5f, 0.5f }, Vec3f::Forward, Vec3f::Zero, { 0.0f, 1.0f } },		// 2.
			{ { -0.5f, 0.5f, 0.5f }, Vec3f::Forward, Vec3f::Zero, { 1.0f, 1.0f } },		// 3.

			// Right face.
			{ { 0.5f, -0.5f, 0.5f }, Vec3f::Right, Vec3f::Zero, { 1.0f, 0.0f } },		// 4.
			{ { 0.5f, -0.5f, -0.5f }, Vec3f::Right, Vec3f::Zero, { 0.0f, 0.0f } },		// 5.
			{ { 0.5f, 0.5f, -0.5f }, Vec3f::Right, Vec3f::Zero, { 0.0f, 1.0f } },		// 6.
			{ { 0.5f, 0.5f, 0.5f }, Vec3f::Right, Vec3f::Zero, { 1.0f, 1.0f } },		// 7.

			// Back face.
			{ { 0.5f, -0.5f, -0.5f }, -Vec3f::Forward, Vec3f::Zero, { 1.0f, 0.0f } },	// 8.
			{ { -0.5f, -0.5f, -0.5f }, -Vec3f::Forward, Vec3f::Zero, { 0.0f, 0.0f } },	// 9.
			{ { -0.5f, 0.5f, -0.5f }, -Vec3f::Forward, Vec3f::Zero, { 0.0f, 1.0f } },	// 10.
			{ { 0.5f, 0.5f, -0.5f }, -Vec3f::Forward, Vec3f::Zero, { 1.0f, 1.0f } },	// 11

			// Left face.
			{ { -0.5f, -0.5f, -0.5f }, -Vec3f::Right, Vec3f::Zero, { 1.0f, 0.0f } },	// 12.
			{ { -0.5f, -0.5f, 0.5f }, -Vec3f::Right, Vec3f::Zero, { 0.0f, 0.0f } },		// 13.
			{ { -0.5f, 0.5f, 0.5f }, -Vec3f::Right, Vec3f::Zero, { 0.0f, 1.0f } },		// 14.
			{ { -0.5f, 0.5f, -0.5f }, -Vec3f::Right, Vec3f::Zero, { 1.0f, 1.0f } },		// 15.

			// Top face.
			{ { -0.5f, 0.5f, 0.5f }, Vec3f::Up, Vec3f::Zero, { 1.0f, 0.0f } },			// 16.
			{ { 0.5f, 0.5f, 0.5f }, Vec3f::Up, Vec3f::Zero, { 0.0f, 0.0f } },			// 17.
			{ { 0.5f, 0.5f, -0.5f }, Vec3f::Up, Vec3f::Zero, { 0.0f, 1.0f } },			// 18.
			{ { -0.5f, 0.5f, -0.5f }, Vec3f::Up, Vec3f::Zero, { 1.0f, 1.0f } },			// 19.

			// Bottom face.
			{ { -0.5f, -0.5f, -0.5f }, -Vec3f::Up, Vec3f::Zero, { 1.0f, 0.0f } },		// 20.
			{ { 0.5f, -0.5f, -0.5f }, -Vec3f::Up, Vec3f::Zero, { 0.0f, 0.0f } },		// 21.
			{ { 0.5f, -0.5f, 0.5f }, -Vec3f::Up, Vec3f::Zero, { 0.0f, 1.0f } },			// 22.
			{ { -0.5f, -0.5f, 0.5f }, -Vec3f::Up, Vec3f::Zero, { 1.0f, 1.0f } },		// 23.
		};

		mesh.indices =
		{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20,
		};

		return mesh;
	}
}