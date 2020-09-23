// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/VkQueueFamilyIndices.hpp>

#include <Rendering/Vulkan/System/VkRenderSurface.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	const uint32* VkQueueFamilyIndices::GetFamilies() const
	{
		return &graphicsFamily;
	}

	void VkQueueFamilyIndices::AddFamily(VkPhysicalDevice _device, const VkRenderSurface& _surface, const VkQueueFamilyProperties& _family, uint32 _index)
	{
		// Graphics family.
		if (_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			graphicsFamily = _index;


		// Present family.
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(_device, _index, _surface, &presentSupport);

		if (presentSupport)
			presentFamily = _index;


		// Transfer family.
		if (_family.queueFlags & VK_QUEUE_TRANSFER_BIT && !(_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !presentSupport)
			transferFamily = _index;
	}

	bool VkQueueFamilyIndices::IsCompleted() const
	{
		return graphicsFamily != uint32(-1) && presentFamily != uint32(-1) && transferFamily != uint32(-1);
	}
}

#endif
