// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	namespace Internal
	{
		template <VertexComp Comps>
		uint32 GetVertexSize()
		{
			if constexpr (Comps == VertexComp::None)
				return 0u;

			else if constexpr (Comps == VertexComp::Position)
				return sizeof(Vec3f);

			else if constexpr (Comps == VertexComp::PN || Comps == VertexComp::PC)
				return 2u * sizeof(Vec3f);

			else if constexpr (Comps == VertexComp::PTex)
				return sizeof(Vec3f) + sizeof(Vec2f);

			else if constexpr (Comps == VertexComp::PNTan || Comps == VertexComp::PNC)
				return 3u * sizeof(Vec3f);

			else if constexpr (Comps == VertexComp::PNTex)
				return 2u * sizeof(Vec3f) + sizeof(Vec2f);

			else if constexpr (Comps == VertexComp::PNTanC)
				return 4u * sizeof(Vec3f);

			else if constexpr (Comps == VertexComp::PNTanTex)
				return 3u * sizeof(Vec3f) + sizeof(Vec2f);

			else if constexpr (Comps == VertexComp::PNTanTexC)
				return 4u * sizeof(Vec3f) + sizeof(Vec2f);
			
			else
			{
				SA_ASSERT(false, InvalidParam, Rendering, L"Invalid vertex layout!")

				return 0u;
			}
		}
	}


	template <VertexComp Comps>
	VertexLayoutSpec<Comps>::VertexLayoutSpec() noexcept :
		VertexLayout{ Comps, Internal::GetVertexSize<Comps>() }
	{
	}


	template <VertexComp Comps>
	Vec3f* VertexLayoutSpec<Comps>::Position(char* _data, uint32 _index) const
	{
		SA_ASSERT(Comps != VertexComp::None, InvalidParam, Rendering, L"Null vertex layout!");

		return reinterpret_cast<Vec3f*>(_data + _index * vertexSize);
	}

	template <VertexComp Comps>
	Vec3f* VertexLayoutSpec<Comps>::Normal(char* _data, uint32 _index) const
	{
		SA_ASSERT(Comps != VertexComp::None, InvalidParam, Rendering, L"Null vertex layout!");

		if constexpr ((Comps & VertexComp::Normal) != VertexComp::None)
			return reinterpret_cast<Vec3f*>(_data + _index * vertexSize + Vertex<Comps>::normalOffset);
		else
			return nullptr;
	}

	template <VertexComp Comps>
	Vec3f* VertexLayoutSpec<Comps>::Tangent(char* _data, uint32 _index) const
	{
		SA_ASSERT(Comps != VertexComp::None, InvalidParam, Rendering, L"Null vertex layout!");

		if constexpr ((Comps & VertexComp::Tangent) != VertexComp::None)
			return reinterpret_cast<Vec3f*>(_data + _index * vertexSize + Vertex<Comps>::tangentOffset);
		else
			return nullptr;
	}

	template <VertexComp Comps>
	Vec2f* VertexLayoutSpec<Comps>::Texture(char* _data, uint32 _index) const
	{
		SA_ASSERT(Comps != VertexComp::None, InvalidParam, Rendering, L"Null vertex layout!");

		if constexpr ((Comps & VertexComp::Texture) != VertexComp::None)
			return reinterpret_cast<Vec2f*>(_data + _index * vertexSize + Vertex<Comps>::textureOffset);
		else
			return nullptr;
	}

	template <VertexComp Comps>
	Vec3f* VertexLayoutSpec<Comps>::Color(char* _data, uint32 _index) const
	{
		SA_ASSERT(Comps != VertexComp::None, InvalidParam, Rendering, L"Null vertex layout!");

		if constexpr ((Comps & VertexComp::Color) != VertexComp::None)
			return reinterpret_cast<Vec3f*>(_data + _index * vertexSize + Vertex<Comps>::colorOffset);
		else
			return nullptr;
	}


	template <VertexComp Comps>
	uint32 VertexLayoutSpec<Comps>::GetPositionOffet() const noexcept
	{
		return 0u;
	}

	template <VertexComp Comps>
	uint32 VertexLayoutSpec<Comps>::GetNormalOffet() const noexcept
	{
		if constexpr ((Comps & VertexComp::Normal) != VertexComp::None)
			return Vertex<Comps>::normalOffset;
		else
			return 0u;
	}

	template <VertexComp Comps>
	uint32 VertexLayoutSpec<Comps>::GetTangentOffet() const noexcept
	{
		if constexpr ((Comps & VertexComp::Tangent) != VertexComp::None)
			return Vertex<Comps>::tangentOffset;
		else
			return 0u;
	}

	template <VertexComp Comps>
	uint32 VertexLayoutSpec<Comps>::GetTextureOffet() const noexcept
	{
		if constexpr ((Comps & VertexComp::Texture) != VertexComp::None)
			return Vertex<Comps>::textureOffset;
		else
			return 0u;
	}

	template <VertexComp Comps>
	uint32 VertexLayoutSpec<Comps>::GetColorOffet() const noexcept
	{
		if constexpr ((Comps & VertexComp::Color) != VertexComp::None)
			return Vertex<Comps>::colorOffset;
		else
			return 0u;
	}
}
