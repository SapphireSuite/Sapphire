// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_DEVICE_GUARD
#define SAPPHIRE_RENDERING_VK_DEVICE_GUARD

#include <Rendering/Config.hpp>

#include <Rendering/Vulkan/System/Device/VkQueueManager.hpp>
#include <Rendering/Vulkan/System/Device/VkQueueFamilyType.hpp>
#include <Rendering/Vulkan/System/Device/VkPhysicalDeviceInfos.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class RenderInstance;
	class RenderSurface;

	class Device
	{
	protected:
		VkDevice mLogicalDevice = VK_NULL_HANDLE;
		VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;

		// Properties.
		VkPhysicalDeviceMemoryProperties mMemProperties;

	public:
		QueueManager queueMgr;

		bool IsValid() const noexcept;

		const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const noexcept;

		void Create(const PhysicalDeviceInfos& _infos);
		void Destroy();

		static std::vector<PhysicalDeviceInfos> QuerySuitableDevices(const RenderInstance& _instance, QueueFamilyType _requiredFamilies, const RenderSurface* _surface = nullptr);

		operator VkDevice() const noexcept;
		operator VkPhysicalDevice() const noexcept;
	};
}

#endif

#endif // GUARD
