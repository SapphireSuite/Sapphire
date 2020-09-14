// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SA_RENDERING_VK_QUEUE_FAMILY_INDICES_GUARD
#define SA_RENDERING_VK_QUEUE_FAMILY_INDICES_GUARD

#include <vulkan/vulkan.h>

#include <Core/Types/Int.hpp>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkRenderSurface;

	class VkQueueFamilyIndices
	{
	public:
		uint32 graphicsFamily = uint32(-1);
		uint32 presentFamily = uint32(-1);

		void AddFamily(VkPhysicalDevice _device, const VkRenderSurface& _surface, const VkQueueFamilyProperties& _family, uint32 _index);
		bool IsCompleted() const;
	};
}

#endif

#endif // GUARD
