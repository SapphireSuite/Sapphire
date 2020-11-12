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
	class SA_ENGINE_API RawMesh
	{
		std::shared_ptr<VertexLayout> mLayout = VertexLayout::Make(VertexComp::Default);


	public:
		std::vector<char> vertices;
		std::vector<uint32> indices;

		RawMesh() = default;
		RawMesh(VertexComp _comps) noexcept;

		std::shared_ptr<VertexLayout> GetLayout() const noexcept;
		void SetLayout(VertexComp _comps);

		void ComputeTangents();

		template <VertexComp Comps = VertexComp::PNTanTex>
		static RawMesh Triangle() noexcept;
		template <VertexComp Comps = VertexComp::PNTanTex>
		static RawMesh Square() noexcept;
		template <VertexComp Comps = VertexComp::PNTanTex>
		static RawMesh Cube() noexcept;
	};
}

#include <Rendering/Framework/Primitives/Mesh/RawMesh.inl>

#endif // GUARD
