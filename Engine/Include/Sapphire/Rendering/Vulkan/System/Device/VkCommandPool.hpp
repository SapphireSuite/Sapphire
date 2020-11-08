// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SA_RENDERING_VK_COMMAND_POOL_GUARD
#define SA_RENDERING_VK_COMMAND_POOL_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa::Vk
{
	class Device;

	class CommandPool
	{
		VkCommandPool mHandle = VK_NULL_HANDLE;

	public:
		void Create(const Device& _device, uint32 _queueFamilyIndex);
		void Destroy(const Device& _device);

		operator VkCommandPool() const;
	};
}

#endif

#endif // GUARD
