// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RAW_MESH_GUARD
#define SAPPHIRE_RENDERING_RAW_MESH_GUARD

#include <vector>

#include <Core/Algorithms/Move.hpp>
#include <Core/Algorithms/SizeOf.hpp>
#include <Core/Algorithms/MemMove.hpp>

#include <Rendering/Framework/Primitives/Mesh/Vertex/VertexLayoutSpec.hpp>

namespace Sa
{
	class RawMesh
	{
		std::shared_ptr<VertexLayout> mLayout = VertexLayout::Make(VertexComp::Default);


	public:
		std::vector<char> vertices;
		std::vector<uint32> indices;

		RawMesh() = default;
		RawMesh(VertexComp _comps) noexcept;

		std::shared_ptr<VertexLayout> GetLayout() const noexcept;
		void SetLayout(VertexComp _comps);

		SA_ENGINE_API void ComputeTangents();

		template <VertexComp Comps = VertexComp::PNTanTex>
		static RawMesh SquareMesh() noexcept;
		template <VertexComp Comps = VertexComp::PNTanTex>
		static RawMesh CubeMesh() noexcept;
	};
}

#include <Rendering/Framework/Primitives/Mesh/RawMesh.inl>

#endif // GUARD
