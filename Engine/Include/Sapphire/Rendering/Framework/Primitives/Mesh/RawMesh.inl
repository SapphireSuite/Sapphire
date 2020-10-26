// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <VertexComp Comps>
	RawMesh RawMesh::SquareMesh() noexcept
	{
		RawMesh mesh;

		mesh.mLayout = VertexLayout::Make<Comps>();

		const Vertex<Comps> vertices[] =
		{
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, -0.5f, 0.0f }, Vec3f::Forward, Vec3f::Zero, { 1.0f, 0.0f }, Vec3f::One },
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, -0.5f, 0.0f }, Vec3f::Forward, Vec3f::Zero, { 0.0f, 0.0f }, Vec3f::One },
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, 0.5f, 0.0f }, Vec3f::Forward, Vec3f::Zero, { 0.0f, 1.0f }, Vec3f::One },
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, 0.5f, 0.0f }, Vec3f::Forward, Vec3f::Zero, { 1.0f, 1.0f }, Vec3f::One },
		};

		constexpr uint32 totalSize = SizeOf(vertices) * sizeof(Vertex<Comps>);

		mesh.vertices.resize(totalSize);
		MemMove(reinterpret_cast<const char*>(vertices), mesh.vertices.data(), totalSize);


		mesh.indices =
		{
			0, 1, 2, 2, 3, 0
		};


		if constexpr ((Comps & VertexComp::Tangent) != VertexComp::None)
			mesh.ComputeTangents();

		return Move(mesh);
	}

	template <VertexComp Comps>
	RawMesh RawMesh::CubeMesh() noexcept
	{
		RawMesh mesh;

		mesh.mLayout = VertexLayout::Make<Comps>();

		const Vertex<Comps> vertices[] =
		{
			// Front face.
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, -0.5f, 0.5f }, Vec3f::Forward, Vec3f::Zero, { 1.0f, 0.0f }, Vec3f::One },	// 0.
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, -0.5f, 0.5f }, Vec3f::Forward, Vec3f::Zero, { 0.0f, 0.0f }, Vec3f::One },	// 1.
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, 0.5f, 0.5f }, Vec3f::Forward, Vec3f::Zero, { 0.0f, 1.0f }, Vec3f::One },		// 2.
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, 0.5f, 0.5f }, Vec3f::Forward, Vec3f::Zero, { 1.0f, 1.0f }, Vec3f::One },	// 3.

			// Right face.
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, -0.5f, 0.5f }, Vec3f::Right, Vec3f::Zero, { 1.0f, 0.0f }, Vec3f::One },		// 4.
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, -0.5f, -0.5f }, Vec3f::Right, Vec3f::Zero, { 0.0f, 0.0f }, Vec3f::One },		// 5.
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, 0.5f, -0.5f }, Vec3f::Right, Vec3f::Zero, { 0.0f, 1.0f }, Vec3f::One },		// 6.
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, 0.5f, 0.5f }, Vec3f::Right, Vec3f::Zero, { 1.0f, 1.0f }, Vec3f::One },		// 7.

			// Back face.
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, -0.5f, -0.5f }, -Vec3f::Forward, Vec3f::Zero, { 1.0f, 0.0f }, Vec3f::One },	// 8.
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, -0.5f, -0.5f }, -Vec3f::Forward, Vec3f::Zero, { 0.0f, 0.0f }, Vec3f::One },	// 9.
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, 0.5f, -0.5f }, -Vec3f::Forward, Vec3f::Zero, { 0.0f, 1.0f }, Vec3f::One },	// 10.
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, 0.5f, -0.5f }, -Vec3f::Forward, Vec3f::Zero, { 1.0f, 1.0f }, Vec3f::One },	// 11

			// Left face.
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, -0.5f, -0.5f }, -Vec3f::Right, Vec3f::Zero, { 1.0f, 0.0f }, Vec3f::One },	// 12.
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, -0.5f, 0.5f }, -Vec3f::Right, Vec3f::Zero, { 0.0f, 0.0f }, Vec3f::One },	// 13.
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, 0.5f, 0.5f }, -Vec3f::Right, Vec3f::Zero, { 0.0f, 1.0f }, Vec3f::One },		// 14.
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, 0.5f, -0.5f }, -Vec3f::Right, Vec3f::Zero, { 1.0f, 1.0f }, Vec3f::One },	// 15.

			// Top face.
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, 0.5f, 0.5f }, Vec3f::Up, Vec3f::Zero, { 1.0f, 0.0f }, Vec3f::One },			// 16.
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, 0.5f, 0.5f }, Vec3f::Up, Vec3f::Zero, { 0.0f, 0.0f }, Vec3f::One },			// 17.
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, 0.5f, -0.5f }, Vec3f::Up, Vec3f::Zero, { 0.0f, 1.0f }, Vec3f::One },			// 18.
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, 0.5f, -0.5f }, Vec3f::Up, Vec3f::Zero, { 1.0f, 1.0f }, Vec3f::One },		// 19.

			// Bottom face.
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, -0.5f, -0.5f }, -Vec3f::Up, Vec3f::Zero, { 1.0f, 0.0f }, Vec3f::One },		// 20.
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, -0.5f, -0.5f }, -Vec3f::Up, Vec3f::Zero, { 0.0f, 0.0f }, Vec3f::One },		// 21.
			Vertex<VertexComp::PNTanTexC>{ { 0.5f, -0.5f, 0.5f }, -Vec3f::Up, Vec3f::Zero, { 0.0f, 1.0f }, Vec3f::One },		// 22.
			Vertex<VertexComp::PNTanTexC>{ { -0.5f, -0.5f, 0.5f }, -Vec3f::Up, Vec3f::Zero, { 1.0f, 1.0f }, Vec3f::One },		// 23.
		};

		constexpr uint32 totalSize = SizeOf(vertices) * sizeof(Vertex<Comps>);
		mesh.vertices.resize(totalSize);
		MemMove(reinterpret_cast<const char*>(vertices), mesh.vertices.data(), totalSize);


		mesh.indices =
		{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20,
		};


		if constexpr ((Comps & VertexComp::Tangent) != VertexComp::None)
			mesh.ComputeTangents();

		return Move(mesh);
	}
}
