// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Device/VkPhysicalDeviceInfos.hpp>

#include <algorithm>

#include <Rendering/Vulkan/System/VkRenderSurface.hpp>

namespace Sa::Vk
{
	constexpr uint32 familyNum = (sizeof(PhysicalDeviceInfos) - sizeof(VkPhysicalDevice)) / sizeof(PhysicalDeviceInfos::FamilyInfos);


	PhysicalDeviceInfos::PhysicalDeviceInfos(VkPhysicalDevice _device, QueueFamilyType _families) noexcept :
		device{ _device },
		families{ _families }
	{
	}


	void PhysicalDeviceInfos::QueryQueueFamilies(const RenderSurface* _surface) noexcept
	{
		SA_ASSERT(device != VK_NULL_HANDLE, InvalidParam, Rendering, L"Query families of a null physical device!");

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		for (uint32 i = 0; i < queueFamilies.size(); ++i)
			AddFamily(_surface, queueFamilies[i], i);
	}

	void PhysicalDeviceInfos::AddFamily(const RenderSurface* _surface,
		const VkQueueFamilyProperties& _family, uint32 _index) noexcept
	{
		// Present family. Should be checked first.
		if (_surface && (static_cast<uint32>(families) & static_cast<uint32>(QueueFamilyType::Present)))
		{
			if (present.index == uint32(-1) ||				// Not completed yet.
				graphics.index != _index)					// Different from graphics.
			{
				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, _index, *_surface, &presentSupport);

				if (presentSupport)
					present.index = _index;
			}
		}


		// Graphics family.
		if ((static_cast<uint32>(families) & static_cast<uint32>(QueueFamilyType::Graphics)) &&
			(_family.queueFlags & VK_QUEUE_GRAPHICS_BIT))
		{
			if (graphics.index == uint32(-1) ||				// Not completed yet.
				graphics.queueNum < _family.queueCount)		// Allow more queue simultaneously.
			{
				graphics.index = _index;
				graphics.queueNum = std::clamp(_family.queueCount, 1u, 3u); // TODO: FIX.
			}
		}


		// Compute family.
		if ((static_cast<uint32>(families) & static_cast<uint32>(QueueFamilyType::Compute)) &&
			(_family.queueFlags & VK_QUEUE_COMPUTE_BIT))
		{
			if (compute.index == uint32(-1) ||				// Not completed yet.
				graphics.index != _index)					// Different from graphics.
			{
				compute.index = _index;
				compute.queueNum = std::clamp(_family.queueCount, 1u, 3u); // TODO: FIX.
			}
		}


		// Transfer family.
		if ((static_cast<uint32>(families) & static_cast<uint32>(QueueFamilyType::Transfer)) &&
			(_family.queueFlags & VK_QUEUE_TRANSFER_BIT))
		{
			if (transfer.index == uint32(-1) ||				// Not completed yet.
				(graphics.index != _index &&				// Different from Graphics.
				present.index != _index))					// Different from Present.
			{
				transfer.index = _index;
				transfer.queueNum = std::clamp(_family.queueCount, 1u, 3u); // TODO: FIX.
			}
		}
	}

	bool PhysicalDeviceInfos::QueueFamiliesCompleted() const noexcept
	{
		const FamilyInfos* data = &graphics;

		// Check each family in type.
		for (uint32 i = 0u; i < familyNum; ++i)
		{
			// Has current type.
			if (static_cast<uint32>(families) & (1 << i))
			{
				// Is family completed.
				if (data[i].index == uint32(-1))
					return false;
			}
		}

		return true;
	}


	std::vector<VkDeviceQueueCreateInfo> PhysicalDeviceInfos::GetDeviceCreateInfos() const noexcept
	{
		std::vector<VkDeviceQueueCreateInfo> result;
		result.reserve(5u);

		static constexpr float queuePriority = 1.0f;
		const FamilyInfos* data = &graphics;

		for (uint32 i = 0u; i < familyNum; ++i)
		{
			// Has current type.
			if (static_cast<uint32>(families) & (1 << i))
			{
				SA_ASSERT(data[i].index != uint32(-1), InvalidParam, Rendering, L"Create device infos of an uncompleted family!");

				VkDeviceQueueCreateInfo queueInfos;
				queueInfos.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueInfos.pNext = nullptr;
				queueInfos.flags = 0;
				queueInfos.queueFamilyIndex = data[i].index;
				queueInfos.queueCount = data[i].queueNum;
				queueInfos.pQueuePriorities = &queuePriority;

				result.emplace_back(queueInfos);
			}
		}

		return result;
	}
}
