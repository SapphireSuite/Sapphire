// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_DEVICE_GUARD
#define SAPPHIRE_RENDERING_VK_DEVICE_GUARD

#include <Rendering/Config.hpp>

#include <Rendering/Vulkan/System/Device/VkQueue.hpp>
#include <Rendering/Vulkan/System/Device/VkQueueFamilyIndices.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class Device
	{
	protected:
		VkDevice mLogicalDevice = VK_NULL_HANDLE;
		VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;

	public:
		void Create(VkPhysicalDevice _device, QueueFamilyIndices& _queueFamilyIndices);
		void Destroy();

		operator VkDevice() const noexcept;
		operator VkPhysicalDevice() const noexcept;
	};
}

#endif

#endif // GUARD
