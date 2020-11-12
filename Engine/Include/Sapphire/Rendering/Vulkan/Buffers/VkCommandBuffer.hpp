// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_COMMAND_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_COMMAND_BUFFER_GUARD

#include <vector>

#include <Rendering/APIConfig.hpp>

#include <Rendering/Vulkan/System/Device/VkQueueType.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class Device;
	class Queue;

	class CommandBuffer
	{
		VkCommandBuffer mHandle = VK_NULL_HANDLE;

		uint32 mPoolIndex = 0u;
		QueueType mQueueType = QueueType::Graphics;

		CommandBuffer(uint32 _poolIndex, QueueType _queueType) noexcept;

	public:
		CommandBuffer() = default;

		uint32 GetPoolIndex() const noexcept;
		const VkCommandBuffer& Get() const noexcept;
		

		static CommandBuffer Allocate(const Device& _device, QueueType _queueType, uint32 _poolIndex = 0u, VkCommandBufferLevel _level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		
		static std::vector<CommandBuffer> AllocateMultiple(const Device& _device, QueueType _queueType, uint32 _num,
			uint32 _poolIndex = 0u, VkCommandBufferLevel _level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		
		static void Free(const Device& _device, CommandBuffer& _commandBuffer);
		
		static void FreeMultiple(const Device& _device, std::vector<CommandBuffer>& _buffers);


		static CommandBuffer BeginSingleTimeCommands(const Device& _device, QueueType _queueType, uint32 _poolIndex = 0u);
		static void EndSingleTimeCommands(const Device& _device, CommandBuffer& _commandBuffer);


		operator VkCommandBuffer() const noexcept;
	};
}

#endif

#endif // GUARD
