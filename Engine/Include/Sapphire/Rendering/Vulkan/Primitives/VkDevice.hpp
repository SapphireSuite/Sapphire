// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_DEVICE_GUARD
#define SAPPHIRE_RENDERING_VK_DEVICE_GUARD

#include <Rendering/Vulkan/Queue/VkQueue.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkRenderSurface;
	class VkQueueFamilyIndices;

	class VkDevice
	{
	protected:
		::VkDevice mLogicalDevice = VK_NULL_HANDLE;
		VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;

		VkQueue mPresentQueue;
		VkQueue mGraphicsQueue;

	public:
		const VkQueue& GetPresentQueue() const noexcept;
		const VkQueue& GetGraphicsQueue() const noexcept;

		bool IsValid() const noexcept;

		void Create(VkPhysicalDevice _device, VkQueueFamilyIndices& _queueFamilyIndices);
		void Destroy();

		static uint32 GetExtensionNum() noexcept;
		static const char* const* GetExtensionNames() noexcept;

		static void QueryQueueFamilies(VkPhysicalDevice _device, const VkRenderSurface& _surface, VkQueueFamilyIndices& _queueFamilyIndices);
		static bool IsPhysicalDeviceSuitable(VkPhysicalDevice _device, const VkRenderSurface& _surface, VkQueueFamilyIndices& _queueFamilyIndices);

		operator ::VkDevice() const noexcept;
		operator VkPhysicalDevice() const noexcept;
	};
}

#endif

#endif // GUARD
