// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VERTEX_LAYOUT_GUARD
#define SAPPHIRE_RENDERING_VERTEX_LAYOUT_GUARD

#include <memory>

#include <Collections/Debug>

#include <Maths/Space/Vector2.hpp>
#include <Maths/Space/Vector3.hpp>

#include <Rendering/Framework/Primitives/Mesh/Vertex/VertexComp.hpp>

namespace Sa
{
	class VertexLayout
	{
	protected:
		VertexLayout() = default;
		SA_ENGINE_API VertexLayout(VertexComp _comps, uint32 _vertexSize) noexcept;

	public:
		const VertexComp comps = VertexComp::None;

		const uint32 vertexSize = 0u;

		virtual ~VertexLayout() = default;


		char* Prev(char* _data) const;
		char* Next(char* _data) const;

		virtual Vec3f* Position(char* _data, uint32 _index = 0u) const = 0;
		virtual Vec3f* Normal(char* _data, uint32 _index = 0u) const = 0;
		virtual Vec3f* Tangent(char* _data, uint32 _index = 0u) const = 0;
		virtual Vec2f* Texture(char* _data, uint32 _index = 0u) const = 0;
		virtual Vec3f* Color(char* _data, uint32 _index = 0u) const = 0;

		virtual uint32 GetPositionOffet() const noexcept = 0;
		virtual uint32 GetNormalOffet() const noexcept = 0;
		virtual uint32 GetTangentOffet() const noexcept = 0;
		virtual uint32 GetTextureOffet() const noexcept = 0;
		virtual uint32 GetColorOffet() const noexcept = 0;

		template <VertexComp Comps>
		static std::shared_ptr<VertexLayout> Make() noexcept;
		SA_ENGINE_API static std::shared_ptr<VertexLayout> Make(VertexComp _comps) noexcept;
	};
}

#include <Rendering/Framework/Primitives/Mesh/Vertex/VertexLayout.inl>

#endif // GUARD
