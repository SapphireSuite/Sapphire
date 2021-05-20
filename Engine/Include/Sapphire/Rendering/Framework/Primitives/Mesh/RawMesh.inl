// Copyright 2020 Sapphire development team. All Rights Reserved.

// TODO: API_ConvertCoordinateSystem should be call in model matrix?
#include <Rendering/Framework/Misc/APISpecific.hpp>

namespace Sa
{
	template <VertexComp Comps>
	RawMesh RawMesh::Triangle() noexcept
	{
		RawMesh mesh;

		mesh.mLayout = VertexLayout::Make<Comps>();

		const Vertex<Comps> vertices[] =
		{
			Vertex<VertexComp::PNTanTexC>{
				API_ConvertCoordinateSystem(Vec3f{ 0.0f, 0.5f, 0.0f }),
				Vec3f::Forward,
				Vec3f::Zero,
				{ 0.5f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, -0.5f, 0.0f }),
				Vec3f::Forward,
				Vec3f::Zero,
				{ 1.0f, 1.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, -0.5f, 0.0f }),
				Vec3f::Forward,
				Vec3f::Zero,
				{ 0.0f, 1.0f },
				Vec3f::One
			},
		};

		constexpr uint32 totalSize = SizeOf(vertices) * sizeof(Vertex<Comps>);

		mesh.vertices.resize(totalSize);
		MemMove(reinterpret_cast<const char*>(vertices), mesh.vertices.data(), totalSize);

		mesh.indices = { 0, 1, 2 };

		if constexpr ((Comps & VertexComp::Tangent) != VertexComp::None)
			mesh.ComputeTangents();

		return Move(mesh);
	}

	template <VertexComp Comps>
	RawMesh RawMesh::Square() noexcept
	{
		RawMesh mesh;

		mesh.mLayout = VertexLayout::Make<Comps>();

		const Vertex<Comps> vertices[] =
		{
			Vertex<VertexComp::PNTanTexC>{
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, 0.5f, 0.0f }),
				Vec3f::Forward,
				Vec3f::Zero,
				{ 0.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, 0.5f, 0.0f }),
				Vec3f::Forward,
				Vec3f::Zero,
				{ 1.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, -0.5f, 0.0f }),
				Vec3f::Forward,
				Vec3f::Zero,
				{ 1.0f, 1.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, -0.5f, 0.0f }),
				Vec3f::Forward,
				Vec3f::Zero,
				{ 0.0f, 1.0f },
				Vec3f::One
			},
		};

		constexpr uint32 totalSize = SizeOf(vertices) * sizeof(Vertex<Comps>);

		mesh.vertices.resize(totalSize);
		MemMove(reinterpret_cast<const char*>(vertices), mesh.vertices.data(), totalSize);

		mesh.indices = { 0, 1, 2, 2, 3, 0 };

		if constexpr ((Comps & VertexComp::Tangent) != VertexComp::None)
			mesh.ComputeTangents();

		return Move(mesh);
	}

	template <VertexComp Comps>
	RawMesh RawMesh::Cube() noexcept
	{
		RawMesh mesh;

		mesh.mLayout = VertexLayout::Make<Comps>();

		const Vertex<Comps> vertices[] =
		{
			// Front face.
			Vertex<VertexComp::PNTanTexC>{											// 0.
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, 0.5f, 0.5f }),
				Vec3f::Forward,
				Vec3f::Zero,
				{ 0.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 1.
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, 0.5f, 0.5f }),
				Vec3f::Forward,
				Vec3f::Zero,
				{ 1.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 2.
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, -0.5f, 0.5f }),
				Vec3f::Forward,
				Vec3f::Zero,
				{ 1.0f, 1.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 3.
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, -0.5f, 0.5f }),
				Vec3f::Forward,
				Vec3f::Zero,
				{ 0.0f, 1.0f },
				Vec3f::One
			},


			// Right face.
			Vertex<VertexComp::PNTanTexC>{											// 4.
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, 0.5f, 0.5f }),
				Vec3f::Right,
				Vec3f::Zero,
				{ 0.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 5.
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, 0.5f, -0.5f }),
				Vec3f::Right,
				Vec3f::Zero,
				{ 1.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 6.
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, -0.5f, -0.5f }),
				Vec3f::Right,
				Vec3f::Zero,
				{ 1.0f, 1.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 7.
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, -0.5f, 0.5f }),
				Vec3f::Right,
				Vec3f::Zero,
				{ 0.0f, 1.0f },
				Vec3f::One
			},


			// Back face.
			Vertex<VertexComp::PNTanTexC>{											// 8
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, 0.5f, -0.5f }),
				-Vec3f::Forward,
				Vec3f::Zero,
				{ 0.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 9.
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, 0.5f, -0.5f }),
				-Vec3f::Forward,
				Vec3f::Zero,
				{ 1.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 10.
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, -0.5f, -0.5f }),
				-Vec3f::Forward,
				Vec3f::Zero,
				{ 1.0f, 1.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 11.
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, -0.5f, -0.5f }),
				-Vec3f::Forward,
				Vec3f::Zero,
				{ 0.0f, 1.0f },
				Vec3f::One
			},


			// Left face.
			Vertex<VertexComp::PNTanTexC>{											// 12.
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, 0.5f, -0.5f }),
				-Vec3f::Right,
				Vec3f::Zero,
				{ 0.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 13.
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, 0.5f, 0.5f }),
				-Vec3f::Right,
				Vec3f::Zero,
				{ 1.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 14.
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, -0.5f, 0.5f }),
				-Vec3f::Right,
				Vec3f::Zero,
				{ 1.0f, 1.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 15.
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, -0.5f, -0.5f }),
				-Vec3f::Right,
				Vec3f::Zero,
				{ 0.0f, 1.0f },
				Vec3f::One
			},


			//Top face.
			Vertex<VertexComp::PNTanTexC>{											// 16.
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, 0.5f, -0.5f }),
				Vec3f::Up,
				Vec3f::Zero,
				{ 0.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 17.
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, 0.5f, -0.5f }),
				Vec3f::Up,
				Vec3f::Zero,
				{ 1.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 18.
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, 0.5f, 0.5f }),
				Vec3f::Up,
				Vec3f::Zero,
				{ 1.0f, 1.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 19.
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, 0.5f, 0.5f }),
				Vec3f::Up,
				Vec3f::Zero,
				{ 0.0f, 1.0f },
				Vec3f::One
			},


			// Bottom face.
			Vertex<VertexComp::PNTanTexC>{											// 20.
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, -0.5f, 0.5f }),
				-Vec3f::Up,
				Vec3f::Zero,
				{ 0.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 21.
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, -0.5f, 0.5f }),
				-Vec3f::Up,
				Vec3f::Zero,
				{ 1.0f, 0.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 22.
				API_ConvertCoordinateSystem(Vec3f{ 0.5f, -0.5f, -0.5f }),
				-Vec3f::Up,
				Vec3f::Zero,
				{ 1.0f, 1.0f },
				Vec3f::One
			},

			Vertex<VertexComp::PNTanTexC>{											// 23.
				API_ConvertCoordinateSystem(Vec3f{ -0.5f, -0.5f, -0.5f }),
				-Vec3f::Up,
				Vec3f::Zero,
				{ 0.0f, 1.0f },
				Vec3f::One
			},

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
