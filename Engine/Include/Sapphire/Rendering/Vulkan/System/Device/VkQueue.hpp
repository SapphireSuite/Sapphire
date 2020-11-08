// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SA_RENDERING_VK_QUEUE_GUARD
#define SA_RENDERING_VK_QUEUE_GUARD

#include <Rendering/Vulkan/System/Device/VkCommandPool.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class Queue
	{
		VkQueue mHandle = VK_NULL_HANDLE;

		CommandPool mCommandPool;

	public:
		void Create(const Device& _device, uint32 _queueFamilyIndex);
		void Destroy(const Device& _device);

		const CommandPool& GetCommandPool() const noexcept;

		operator VkQueue() const noexcept;
	};
}

#endif

#endif // GUARD
