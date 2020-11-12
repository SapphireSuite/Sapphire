// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_RENDERING_VERTEX_GUARD
#define SAPPHIRE_RENDERING_VERTEX_GUARD

#include <Rendering/Framework/Primitives/Mesh/Vertex/VertexBase.hpp>

namespace Sa
{
	template <VertexComp Comps>
	struct Vertex : public Internal::VertexBase<Comps>
	{
		using Internal::VertexBase<Comps>::VertexBase;
		using Base = Internal::VertexBase<Comps>;

		Vertex() = default;
		Vertex(Vertex&&) = default;
		Vertex(const Vertex&) = default;

		template <VertexComp inComps>
		Vertex(const Vertex<inComps>& _other);

		Vertex& operator=(Vertex&&) = default;
		Vertex& operator=(const Vertex&) = default;

		template <VertexComp inComps>
		Vertex& operator=(const Vertex<inComps>& _rhs);


		bool operator==(const Vertex& _rhs) const noexcept;
		bool operator!=(const Vertex& _rhs) const noexcept;
	};
}

#include <Rendering/Framework/Primitives/Mesh/Vertex/Vertex.inl>

#endif // GUARD
