// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SA_RENDERING_VK_COMMAND_POOL_GUARD
#define SA_RENDERING_VK_COMMAND_POOL_GUARD

#include <vulkan/vulkan.h>

#include <Core/Types/Int.hpp>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkDevice;

	class VkCommandPool
	{
		::VkCommandPool mHandle = VK_NULL_HANDLE;

	public:
		void Create(const VkDevice& _device, uint32 _queueFamilyIndex);
		void Destroy(const VkDevice& _device);

		operator ::VkCommandPool() const;
	};
}

#endif

#endif // GUARD
