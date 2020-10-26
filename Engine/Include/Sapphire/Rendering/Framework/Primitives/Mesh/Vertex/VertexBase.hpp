// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_RENDERING_VERTEX_BASE_GUARD
#define SAPPHIRE_RENDERING_VERTEX_BASE_GUARD

#include <Maths/Space/Vector2.hpp>
#include <Maths/Space/Vector3.hpp>

#include <Rendering/Framework/Primitives/Mesh/Vertex/VertexComp.hpp>

namespace Sa
{
	namespace Internal
	{
		template <VertexComp Comps>
		struct VertexBase;

		template <>
		struct VertexBase<VertexComp::None>;

		template <>
		struct VertexBase<VertexComp::Position>
		{
			/// Position component of the Vertex.
			Vec3f position;


			/**
			*	\brief \e Default constructor.
			*/
			VertexBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			VertexBase(VertexBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			VertexBase(const VertexBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*/
			SA_ENGINE_API VertexBase(const Vec3f& _position) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			VertexBase& operator=(VertexBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			VertexBase& operator=(const VertexBase&) = default;
		};

		template <>
		struct VertexBase<VertexComp::Position | VertexComp::Normal>
		{
			/// Position component of the Vertex.
			Vec3f position;

			/// Normal component of the Vertex.
			Vec3f normal;

			static constexpr uint32 normalOffset = sizeof(Vec3f);

			/**
			*	\brief \e Default constructor.
			*/
			VertexBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			VertexBase(VertexBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			VertexBase(const VertexBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*	\param[in] _normal		Normal value.
			*/
			SA_ENGINE_API VertexBase(const Vec3f& _position, const Vec3f& _normal) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			VertexBase& operator=(VertexBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			VertexBase& operator=(const VertexBase&) = default;
		};

		template <>
		struct VertexBase<VertexComp::Position | VertexComp::Texture>
		{
			/// Position component of the Vertex.
			Vec3f position;

			/// Texture component of the Vertex.
			Vec2f texture;

			static constexpr uint32 textureOffset = sizeof(Vec3f);

			/**
			*	\brief \e Default constructor.
			*/
			VertexBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			VertexBase(VertexBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			VertexBase(const VertexBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*	\param[in] _texture		Texture value.
			*/
			SA_ENGINE_API 	VertexBase(const Vec3f& _position, const Vec2f& _texture) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			VertexBase& operator=(VertexBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			VertexBase& operator=(const VertexBase&) = default;
		};

		template <>
		struct VertexBase<VertexComp::Position | VertexComp::Color>
		{
			/// Position component of the Vertex.
			Vec3f position;

			/// Color component of the Vertex.
			Vec3f color;

			static constexpr uint32 colorOffset = sizeof(Vec3f);

			/**
			*	\brief \e Default constructor.
			*/
			VertexBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			VertexBase(VertexBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			VertexBase(const VertexBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*	\param[in] _color		Color value.
			*/
			SA_ENGINE_API VertexBase(const Vec3f& _position, const Vec3f& _color) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			VertexBase& operator=(VertexBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			VertexBase& operator=(const VertexBase&) = default;
		};

		template <>
		struct VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Tangent>
		{
			/// Position component of the Vertex.
			Vec3f position;

			/// Normal component of the Vertex.
			Vec3f normal;

			/// Tangent component of the Vertex.
			Vec3f tangent;

			static constexpr uint32 normalOffset = sizeof(Vec3f);
			static constexpr uint32 tangentOffset = 2u * sizeof(Vec3f);

			/**
			*	\brief \e Default constructor.
			*/
			VertexBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			VertexBase(VertexBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			VertexBase(const VertexBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*	\param[in] _normal		Normal value.
			*	\param[in] _tangent		Tangent value.
			*/
			SA_ENGINE_API VertexBase(const Vec3f& _position, const Vec3f& _normal, const Vec3f& _tangent) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			VertexBase& operator=(VertexBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			VertexBase& operator=(const VertexBase&) = default;
		};

		template <>
		struct VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Texture>
		{
			/// Position component of the Vertex.
			Vec3f position;

			/// Normal component of the Vertex.
			Vec3f normal;

			/// Texture component of the Vertex.
			Vec2f texture;

			static constexpr uint32 normalOffset = sizeof(Vec3f);
			static constexpr uint32 textureOffset = 2u * sizeof(Vec3f);

			/**
			*	\brief \e Default constructor.
			*/
			VertexBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			VertexBase(VertexBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			VertexBase(const VertexBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*	\param[in] _normal		Normal value.
			*	\param[in] _texture		Texture value.
			*/
			SA_ENGINE_API VertexBase(const Vec3f& _position, const Vec3f& _normal, const Vec2f& _texture) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			VertexBase& operator=(VertexBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			VertexBase& operator=(const VertexBase&) = default;
		};

		template <>
		struct VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Color>
		{
			/// Position component of the Vertex.
			Vec3f position;

			/// Normal component of the Vertex.
			Vec3f normal;

			/// Color component of the Vertex.
			Vec3f color;

			static constexpr uint32 normalOffset = sizeof(Vec3f);
			static constexpr uint32 colorOffset = 2u * sizeof(Vec3f);

			/**
			*	\brief \e Default constructor.
			*/
			VertexBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			VertexBase(VertexBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			VertexBase(const VertexBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*	\param[in] _normal		Normal value.
			*	\param[in] _color		Color value.
			*/
			SA_ENGINE_API VertexBase(const Vec3f& _position, const Vec3f& _normal, const Vec3f& _color) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			VertexBase& operator=(VertexBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			VertexBase& operator=(const VertexBase&) = default;
		};

		template <>
		struct VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Tangent | VertexComp::Texture>
		{
			/// Position component of the Vertex.
			Vec3f position;

			/// Normal component of the Vertex.
			Vec3f normal;

			/// Tangent component of the Vertex.
			Vec3f tangent;

			/// Texture component of the Vertex.
			Vec2f texture;

			static constexpr uint32 normalOffset = sizeof(Vec3f);
			static constexpr uint32 tangentOffset = 2u * sizeof(Vec3f);
			static constexpr uint32 textureOffset = 3u * sizeof(Vec3f);

			/**
			*	\brief \e Default constructor.
			*/
			VertexBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			VertexBase(VertexBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			VertexBase(const VertexBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*	\param[in] _normal		Normal value.
			*	\param[in] _tangent		Tangent value.
			*	\param[in] _texture		Texture value.
			*/
			SA_ENGINE_API VertexBase(const Vec3f& _position, const Vec3f& _normal, const Vec3f& _tangent, const Vec2f& _texture) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			VertexBase& operator=(VertexBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			VertexBase& operator=(const VertexBase&) = default;
		};

		template <>
		struct VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Tangent | VertexComp::Color>
		{
			/// Position component of the Vertex.
			Vec3f position;

			/// Normal component of the Vertex.
			Vec3f normal;

			/// Tangent component of the Vertex.
			Vec3f tangent;

			/// Color component of the Vertex.
			Vec3f color;

			static constexpr uint32 normalOffset = sizeof(Vec3f);
			static constexpr uint32 tangentOffset = 2u * sizeof(Vec3f);
			static constexpr uint32 colorOffset = 3u * sizeof(Vec3f);

			/**
			*	\brief \e Default constructor.
			*/
			VertexBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			VertexBase(VertexBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			VertexBase(const VertexBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*	\param[in] _normal		Normal value.
			*	\param[in] _tangent		Tangent value.
			*	\param[in] _color		Color value.
			*/
			SA_ENGINE_API VertexBase(const Vec3f& _position, const Vec3f& _normal, const Vec3f& _tangent, const Vec3f& _color) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			VertexBase& operator=(VertexBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			VertexBase& operator=(const VertexBase&) = default;
		};

		template <>
		struct VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Texture | VertexComp::Color>
		{
			/// Position component of the Vertex.
			Vec3f position;

			/// Normal component of the Vertex.
			Vec3f normal;

			/// Texture component of the Vertex.
			Vec2f texture;

			/// color component of the Vertex.
			Vec3f color;

			static constexpr uint32 normalOffset = sizeof(Vec3f);
			static constexpr uint32 textureOffset = 2u * sizeof(Vec3f);
			static constexpr uint32 colorOffset = 2u * sizeof(Vec3f) + sizeof(Vec2f);

			/**
			*	\brief \e Default constructor.
			*/
			VertexBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			VertexBase(VertexBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			VertexBase(const VertexBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*	\param[in] _normal		Normal value.
			*	\param[in] _tangent		Tangent value.
			*	\param[in] _color		Color value.
			*/
			SA_ENGINE_API VertexBase(const Vec3f& _position, const Vec3f& _normal, const Vec2f& _texture, const Vec3f& _color) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			VertexBase& operator=(VertexBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			VertexBase& operator=(const VertexBase&) = default;
		};


		template <>
		struct VertexBase<VertexComp::Position | VertexComp::Normal | VertexComp::Tangent | VertexComp::Texture | VertexComp::Color>
		{
			/// Position component of the Vertex.
			Vec3f position;

			/// Normal component of the Vertex.
			Vec3f normal;

			/// Tangent component of the Vertex.
			Vec3f tangent;

			/// Texture component of the Vertex.
			Vec2f texture;

			/// Color component of the Vertex.
			Vec3f color;

			static constexpr uint32 normalOffset = sizeof(Vec3f);
			static constexpr uint32 tangentOffset = 2u * sizeof(Vec3f);
			static constexpr uint32 textureOffset = 3u * sizeof(Vec3f);
			static constexpr uint32 colorOffset = 3u * sizeof(Vec3f) + sizeof(Vec2f);

			/**
			*	\brief \e Default constructor.
			*/
			VertexBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			VertexBase(VertexBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			VertexBase(const VertexBase&) = default;
			
			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*	\param[in] _normal		Normal value.
			*	\param[in] _tangent		Tangent value.
			*	\param[in] _color		Color value.
			*	\param[in] _texture		Texture value.
			*/
			SA_ENGINE_API VertexBase(const Vec3f& _position, const Vec3f& _normal, const Vec3f& _tangent, const Vec2f& _texture, const Vec3f& _color) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			VertexBase& operator=(VertexBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			VertexBase& operator=(const VertexBase&) = default;
		};
	}
}

#endif // GUARD
