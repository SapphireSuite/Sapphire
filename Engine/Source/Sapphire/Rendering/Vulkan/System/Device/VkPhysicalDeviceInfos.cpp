// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Device/VkPhysicalDeviceInfos.hpp>

#include <algorithm>

#include <Rendering/Vulkan/System/Surface/VkRenderSurface.hpp>

namespace Sa::Vk
{
	constexpr uint32 familyNum = (sizeof(PhysicalDeviceInfos) - sizeof(VkPhysicalDevice)) / sizeof(PhysicalDeviceInfos::FamilyInfos);


	PhysicalDeviceInfos::PhysicalDeviceInfos(VkPhysicalDevice _device, QueueType _familyTypes) noexcept :
		device{ _device },
		familyTypes{ _familyTypes }
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
		if (_surface && (static_cast<uint32>(familyTypes) & static_cast<uint32>(QueueType::Present)))
		{
			if (present.index == ~uint32() ||				// Not completed yet.
				graphics.index != _index)					// Different from graphics.
			{
				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, _index, *_surface, &presentSupport);

				if (presentSupport)
					present.index = _index;
			}
		}


		// Graphics family.
		if ((static_cast<uint32>(familyTypes) & static_cast<uint32>(QueueType::Graphics)) &&
			(_family.queueFlags & VK_QUEUE_GRAPHICS_BIT))
		{
			if (graphics.index == ~uint32() ||					// Not completed yet.
				graphics.maxQueueNum < _family.queueCount)		// Allow more queue simultaneously.
			{
				graphics.index = _index;
				graphics.maxQueueNum = _family.queueCount;
			}
		}


		// Compute family.
		if ((static_cast<uint32>(familyTypes) & static_cast<uint32>(QueueType::Compute)) &&
			(_family.queueFlags & VK_QUEUE_COMPUTE_BIT))
		{
			if (compute.index == ~uint32() ||					// Not completed yet.
				present.index != _index)						// Different from Present.
			{
				compute.index = _index;
				compute.maxQueueNum = _family.queueCount;
			}
		}


		// Transfer family.
		if ((static_cast<uint32>(familyTypes) & static_cast<uint32>(QueueType::Transfer)) &&
			(_family.queueFlags & VK_QUEUE_TRANSFER_BIT))
		{
			if (transfer.index == ~uint32() ||				// Not completed yet.
				(graphics.index != _index &&				// Different from Graphics.
				present.index != _index))					// Different from Present.
			{
				transfer.index = _index;
				transfer.maxQueueNum = _family.queueCount;
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
			if (static_cast<uint32>(familyTypes) & (1 << i))
			{
				// Is family completed.
				if (data[i].index == ~uint32())
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
			if (static_cast<uint32>(familyTypes) & (1 << i))
			{
				SA_ASSERT(data[i].index != ~uint32(), InvalidParam, Rendering, L"Create device infos of an uncompleted family!");


				// Family index already in results.
				bool bFound = false;
				for (auto it = result.begin(); it != result.end(); ++it)
				{
					// Family index found.
					if (it->queueFamilyIndex == data[i].index)
					{
						// Add queueNum.
						it->queueCount = std::max(it->queueCount + data[i].queueNum, data[i].maxQueueNum);

						bFound = true;
					}
				}

				if (bFound)
					continue;


				// Create new queue create infos.
				VkDeviceQueueCreateInfo queueInfos{};
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
