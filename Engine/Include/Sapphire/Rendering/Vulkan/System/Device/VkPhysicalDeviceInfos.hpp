// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_PHYSICAL_DEVICE_INFOS_GUARD
#define SAPPHIRE_RENDERING_VK_PHYSICAL_DEVICE_INFOS_GUARD

#include <vector>

#include <Rendering/Vulkan/System/Device/VkQueueFamilyType.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa::Vk
{
	class RenderSurface;

	struct PhysicalDeviceInfos
	{
		struct FamilyInfos
		{
			uint32 index = uint32(-1);
			uint32 maxQueueNum = 1u;
			mutable uint32 queueNum = 1u;
		};

		const VkPhysicalDevice device = VK_NULL_HANDLE;
		const QueueFamilyType familyTypes;

		FamilyInfos graphics;
		FamilyInfos compute;
		FamilyInfos transfer;
		FamilyInfos present;

		PhysicalDeviceInfos(VkPhysicalDevice _device, QueueFamilyType _familyTypes) noexcept;
		PhysicalDeviceInfos(PhysicalDeviceInfos&&) = default;
		PhysicalDeviceInfos(const PhysicalDeviceInfos&) = default;

		void QueryQueueFamilies(const RenderSurface* _surface) noexcept;
		void AddFamily(const RenderSurface* _surface, const VkQueueFamilyProperties& _family, uint32 _index) noexcept;

		bool QueueFamiliesCompleted() const noexcept;

		std::vector<VkDeviceQueueCreateInfo> GetDeviceCreateInfos() const noexcept;
	};
}

#endif

#endif // GUARD
