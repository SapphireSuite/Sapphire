// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System//Device/VkQueueFamilyIndices.hpp>

#include <Collections/Debug>

#include <Rendering/Vulkan/System/VkRenderSurface.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	const uint32* QueueFamilyIndices::Data() const noexcept
	{
		return &graphicsFamily;
	}

	void QueueFamilyIndices::AddFamily(const VkQueueFamilyProperties& _family, uint32 _index)
	{
		// Graphics family.
		if (static_cast<uint32>(type) & static_cast<uint32>(QueueFamilyType::Graphics) &&
			(_family.queueFlags & VK_QUEUE_GRAPHICS_BIT))
			graphicsFamily = _index;


		// Compute family.
		if (static_cast<uint32>(type) & static_cast<uint32>(QueueFamilyType::Compute) &&
			(_family.queueFlags & VK_QUEUE_COMPUTE_BIT))
			computeFamily = _index;


		// Transfer family.
		if (static_cast<uint32>(type) & static_cast<uint32>(QueueFamilyType::Transfer) &&
			(_family.queueFlags & VK_QUEUE_TRANSFER_BIT))
		{
			if (transferFamily == uint32(-1) ||						// Not completed yet.
				!(_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&	// Can be different from Graphics.
				presentFamily != _index)							// Can be different from Present.
				transferFamily = _index;
		}
	}

	void QueueFamilyIndices::AddFamily(VkPhysicalDevice _device, const RenderSurface& _surface, const VkQueueFamilyProperties& _family, uint32 _index)
	{
		// Present family.
		if (static_cast<uint32>(type) & static_cast<uint32>(QueueFamilyType::Present))
		{
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(_device, _index, _surface, &presentSupport);

			if (presentSupport)
				presentFamily = _index;
		}

		AddFamily(_family, _index);
	}

	bool QueueFamilyIndices::IsCompleted() const noexcept
	{
		const uint32* data = Data();
		constexpr uint32 familyNum = sizeof(QueueFamilyIndices) - sizeof(QueueFamilyType);

		// Check each family in type.
		for (uint32 i = 0u; i < familyNum; ++i)
		{
			// Has current type.
			if (static_cast<uint32>(type) & (i + 1)) // QueueFamilyType starts at 1.
			{
				// Is family completed.
				if (data[i] == uint32(-1))
					return false;
			}
		}

		return true;
	}
}

#endif
