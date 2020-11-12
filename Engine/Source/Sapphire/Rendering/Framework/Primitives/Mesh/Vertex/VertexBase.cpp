// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Mesh/Vertex/VertexBase.hpp>

namespace Sa
{
	namespace Internal
	{
		// === VertexBase P ===
		VertexBase<VertexComp::Position>::VertexBase(const Vec3f& _position) noexcept : position{ _position }
		{
		}


		// === VertexBase PN ===
		VertexBase<VertexComp::Position | VertexComp::Normal>::VertexBase(const Vec3f& _position, const Vec3f& _normal) noexcept :
			position{ _position },
			normal{ _normal }
		{
		}


		// === VertexBase PTex ===
		VertexBase<VertexComp::Position | VertexComp::Texture>::VertexBase(const Vec3f& _position, const Vec2f& _texture) noexcept :
			position{ _position },
			texture{ _texture }
		{
		}


		// === VertexBase PC ===
		VertexBase<VertexComp::Position | VertexComp::Color>::VertexBase(const Vec3f& _position, const Vec3f& _color) noexcept :
			position{ _position },
			color{ _color }
		{
		}

		
		// === VertexBase PNTan ===
		VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Tangent>::VertexBase(const Vec3f& _position, const Vec3f& _normal, const Vec3f& _tangent) noexcept :
			position{ _position },
			normal{ _normal },
			tangent{ _tangent }
		{
		}


		// === VertexBase PNTex ===
		VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Texture>::VertexBase(const Vec3f& _position, const Vec3f& _normal, const Vec2f& _texture) noexcept :
			position{ _position },
			normal{ _normal },
			texture{ _texture }
		{
		}


		// === VertexBase PNC ===
		VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Color>::VertexBase(const Vec3f& _position, const Vec3f& _normal, const Vec3f& _color) noexcept :
			position{ _position },
			normal{ _normal },
			color{ _color }
		{
		}



		// === VertexBase PNTanTex ===
		VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Tangent | VertexComp::Texture>::VertexBase(const Vec3f& _position,
			const Vec3f& _normal, const Vec3f& _tangent, const Vec2f& _texture) noexcept :
			position{ _position },
			normal{ _normal },
			tangent{ _tangent },
			texture{ _texture }
		{
		}


		// === VertexBase PNTanC ===
		VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Tangent | VertexComp::Color>::VertexBase(const Vec3f& _position,
			const Vec3f& _normal, const Vec3f& _tangent, const Vec3f& _color) noexcept :
			position{ _position },
			normal{ _normal },
			tangent{ _tangent },
			color{ _color }
		{
		}


		// === VertexBase PNTexC ===
		VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Texture | VertexComp::Color>::VertexBase(const Vec3f& _position,
			const Vec3f& _normal, const Vec2f& _texture, const Vec3f& _color) noexcept :
			position{ _position },
			normal{ _normal },
			texture{ _texture },
			color{ _color }
		{
		}


		// === VertexBase PNTanTexC ===
		VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Tangent | VertexComp::Texture | VertexComp::Color>::VertexBase(const Vec3f& _position,
			const Vec3f& _normal, const Vec3f& _tangent, const Vec2f& _texture, const Vec3f& _color) noexcept :
			position{ _position },
			normal{ _normal },
			tangent{ _tangent },
			texture{ _texture },
			color{ _color }
		{
		}
	}
}
