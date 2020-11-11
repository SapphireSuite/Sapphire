// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SA_RENDERING_VK_QUEUE_GUARD
#define SA_RENDERING_VK_QUEUE_GUARD

#include <vector>

#include <Rendering/Vulkan/Buffers/VkCommandBuffer.hpp>
#include <Rendering/Vulkan/System/Device/VkCommandPool.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class SA_ENGINE_API Queue
	{
		uint32 mFamilyIndex = uint32(-1);

		std::vector<VkQueue> mHandles;

		/**
		*	Use 1 CommandPool per thread.
		*	Sources: https://developer.nvidia.com/blog/vulkan-dos-donts/
		*/
		std::vector<CommandPool> mCommandPools;

	public:
		uint32 GetFamilyIndex() const noexcept;
		
		bool IsValid() const noexcept;

		VkQueue GetHandle(uint32 _index) const;
		const CommandPool& GetCommandPool(uint32 _index) const;

		void Create(const Device& _device, uint32 _queueFamilyIndex, uint32 _queueNum = 1u);
		void Destroy(const Device& _device);
	};
}

#endif

#endif // GUARD
