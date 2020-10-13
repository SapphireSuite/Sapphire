// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_RENDERING_VERTEX_GUARD
#define SAPPHIRE_RENDERING_VERTEX_GUARD

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

#endif

#include <Maths/Space/Vector2.hpp>
#include <Maths/Space/Vector3.hpp>

namespace Sa
{
	class Vertex
	{
	public:
		Vec3f position;
		Vec3f normal;
		Vec3f tangent;
		Vec3f bitangent;
		Vec2f texture;

		bool operator==(const Vertex& _rhs) const noexcept;
		bool operator!=(const Vertex& _rhs) const noexcept;

#if SA_RENDERING_API == SA_VULKAN

		static constexpr uint32 bindingDescriptionNum = 1u;

		static constexpr uint32 attributeDescriptionNum = 5u;

		static const VkVertexInputBindingDescription* GetBindingDescription() noexcept;

		static const VkVertexInputAttributeDescription* GetAttributeDescriptions() noexcept;

#endif
	};
}

#endif // GUARD
