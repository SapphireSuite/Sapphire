// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SA_RENDERING_VK_QUEUE_GUARD
#define SA_RENDERING_VK_QUEUE_GUARD

#include <Rendering/Vulkan/Queue/VkCommandPool.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkQueue
	{
		::VkQueue mHandle = VK_NULL_HANDLE;

		VkCommandPool mCommandPool;

	public:
		void Create(const VkDevice& _device, uint32 _queueFamilyIndex);
		void Destroy(const VkDevice& _device);

		const VkCommandPool& GetCommandPool() const noexcept;

		operator ::VkQueue() const noexcept;
	};
}

#endif

#endif // GUARD
