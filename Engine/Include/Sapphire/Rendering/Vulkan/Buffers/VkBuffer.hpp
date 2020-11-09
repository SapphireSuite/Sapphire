// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_BUFFER_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa::Vk
{
	class Device;

	class Buffer
	{
		VkBuffer mHandle = VK_NULL_HANDLE;
		VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE;

	public:
		static uint32 FindMemoryType(const Device& _device, uint32 _typeFilter, VkMemoryPropertyFlags _properties);
	};
}

#endif

#endif // GUARD
