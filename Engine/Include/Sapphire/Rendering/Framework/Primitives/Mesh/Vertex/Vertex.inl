// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <VertexComp Comps>
	template <VertexComp inComps>
	Vertex<Comps>::Vertex(const Vertex<inComps>& _other)
	{
		// Call assignment operator.
		*this = _other;
	}


	template <VertexComp Comps>
	template <VertexComp inComps>
	Vertex<Comps>& Vertex<Comps>::operator=(const Vertex<inComps>& _rhs)
	{
		if constexpr ((Comps & inComps & VertexComp::Position) != VertexComp::None)
			Base::position = _rhs.position;

		if constexpr ((Comps & inComps & VertexComp::Normal) != VertexComp::None)
			Base::normal = _rhs.normal;

		if constexpr ((Comps & inComps & VertexComp::Tangent) != VertexComp::None)
			Base::tangent = _rhs.tangent;

		if constexpr ((Comps & inComps & VertexComp::Color) != VertexComp::None)
			Base::color = _rhs.color;

		if constexpr ((Comps & inComps & VertexComp::Texture) != VertexComp::None)
			Base::texture = _rhs.texture;

		return *this;
	}


	template <VertexComp Comps>
	bool Vertex<Comps>::operator==(const Vertex& _rhs) const noexcept
	{
		if constexpr ((Comps & VertexComp::Position) != VertexComp::None)
		{
			if (Base::position != _rhs.position)
				return false;
		}

		if constexpr ((Comps & VertexComp::Normal) != VertexComp::None)
		{
			if (Base::normal != _rhs.normal)
				return false;
		}

		if constexpr ((Comps & VertexComp::Tangent) != VertexComp::None)
		{
			if (Base::tangent != _rhs.tangent)
				return false;
		}

		if constexpr ((Comps & VertexComp::Texture) != VertexComp::None)
		{
			if (Base::texture != _rhs.texture)
				return false;
		}

		if constexpr ((Comps & VertexComp::Color) != VertexComp::None)
		{
			if (Base::color != _rhs.color)
				return false;
		}

		return true;
	}

	template <VertexComp Comps>
	bool Vertex<Comps>::operator!=(const Vertex& _rhs) const noexcept
	{
		return !(*this == _rhs);
	}
}
