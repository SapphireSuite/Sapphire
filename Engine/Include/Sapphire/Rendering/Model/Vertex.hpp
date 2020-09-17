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
		//Vec2f texture;
		Vec3f color;

#if SA_RENDERING_API == SA_VULKAN

		static constexpr uint32 bindingDescriptionNum = 1u;

		static constexpr uint32 attributeDescriptionNum = 3u;

		static const VkVertexInputBindingDescription* GetBindingDescription() noexcept;

		static const VkVertexInputAttributeDescription* GetAttributeDescriptions() noexcept;

#endif
	};
}

#endif // GUARD
