// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_DEVICE_GUARD
#define SAPPHIRE_RENDERING_VK_DEVICE_GUARD

#include <Rendering/Vulkan/System/VkQueue.hpp>
#include <Rendering/Vulkan/System/VkQueueFamilyIndices.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkRenderSurface;

	class VkDevice
	{
	protected:
		::VkDevice mLogicalDevice = VK_NULL_HANDLE;
		VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;

		VkQueue mPresentQueue;
		VkQueue mGraphicsQueue;
		VkQueue mTransferQueue;

		VkQueueFamilyIndices mQueueFamilyIndices;

		static void QueryQueueFamilies(VkPhysicalDevice _device, const VkRenderSurface& _surface, VkQueueFamilyIndices& _queueFamilyIndices);

	public:
		const VkQueue& GetPresentQueue() const noexcept;
		// TODO: REMOVE LATER.
		SA_ENGINE_API const VkQueue& GetGraphicsQueue() const noexcept;
		const VkQueue& GetTransferQueue() const noexcept;

		SA_ENGINE_API const VkQueueFamilyIndices& GetQueueFamilyIndices() const;

		bool IsValid() const noexcept;

		void Create(VkPhysicalDevice _device, VkQueueFamilyIndices& _queueFamilyIndices);
		void Destroy();

		static uint32 GetExtensionNum() noexcept;
		static const char* const* GetExtensionNames() noexcept;

		static bool IsPhysicalDeviceSuitable(VkPhysicalDevice _device, const VkRenderSurface& _surface, VkQueueFamilyIndices& _queueFamilyIndices);

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API operator ::VkDevice() const noexcept;
		operator VkPhysicalDevice() const noexcept;
	};
}

#endif

#endif // GUARD
