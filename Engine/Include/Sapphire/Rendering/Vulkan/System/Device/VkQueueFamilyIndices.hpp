// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SA_RENDERING_VK_QUEUE_FAMILY_INDICES_GUARD
#define SA_RENDERING_VK_QUEUE_FAMILY_INDICES_GUARD

#include <Rendering/Vulkan/System/Device/VkQueueFamilyType.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa::Vk
{
	class RenderSurface;

	struct QueueFamilyIndices
	{
		QueueFamilyType type = QueueFamilyType::Graphics;

		uint32 graphicsFamily = uint32(-1);
		uint32 computeFamily = uint32(-1);
		uint32 transferFamily = uint32(-1);
		uint32 presentFamily = uint32(-1);

		const uint32* Data() const noexcept;

		void AddFamily(const VkQueueFamilyProperties& _family, uint32 _index);
		void AddFamily(VkPhysicalDevice _device, const RenderSurface& _surface, const VkQueueFamilyProperties& _family, uint32 _index);

		bool IsCompleted() const noexcept;
	};
}

#endif

#endif // GUARD
