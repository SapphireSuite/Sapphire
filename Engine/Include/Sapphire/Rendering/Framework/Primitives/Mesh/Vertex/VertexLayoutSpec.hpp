// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VERTEX_LAYOUT_SPEC_GUARD
#define SAPPHIRE_RENDERING_VERTEX_LAYOUT_SPEC_GUARD

#include <Rendering/Framework/Primitives/Mesh/Vertex/VertexLayout.hpp>

#include <Rendering/Framework/Primitives/Mesh/Vertex/Vertex.hpp>

namespace Sa
{
	template <VertexComp Comps>
	class VertexLayoutSpec : public VertexLayout
	{
	public:
		VertexLayoutSpec() noexcept;

		Vec3f* Position(char* _data, uint32 _index) const override final;
		Vec3f* Normal(char* _data, uint32 _index) const override final;
		Vec3f* Tangent(char* _data, uint32 _index) const override final;
		Vec2f* Texture(char* _data, uint32 _index) const override final;
		Vec3f* Color(char* _data, uint32 _index) const override final;

		uint32 GetPositionOffet() const noexcept override final;
		uint32 GetNormalOffet() const noexcept override final;
		uint32 GetTangentOffet() const noexcept override final;
		uint32 GetTextureOffet() const noexcept override final;
		uint32 GetColorOffet() const noexcept override final;
	};
}

#include <Rendering/Framework/Primitives/Mesh/Vertex/VertexLayoutSpec.inl>

#endif // GUARD
