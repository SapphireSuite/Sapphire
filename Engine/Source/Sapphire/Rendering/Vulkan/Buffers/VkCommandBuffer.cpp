// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffers/VkCommandBuffer.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/Device/VkDevice.hpp>
#include <Rendering/Vulkan/System/Device/VkQueue.hpp>

namespace Sa::Vk
{
	CommandBuffer::CommandBuffer(uint32 _poolIndex, QueueType _queueType) noexcept :
		mPoolIndex{ _poolIndex },
		mQueueType{ _queueType }
	{
	}

	uint32 CommandBuffer::GetPoolIndex() const noexcept
	{
		return mPoolIndex;
	}

	const VkCommandBuffer& CommandBuffer::Get() const noexcept
	{
		return mHandle;
	}

	CommandBuffer CommandBuffer::Allocate(const Device& _device, QueueType _queueType, uint32 _poolIndex, VkCommandBufferLevel _level)
	{
		CommandBuffer result(_poolIndex, _queueType);

		VkCommandBufferAllocateInfo commandBufferAllocInfo{};
		commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocInfo.pNext = nullptr;
		commandBufferAllocInfo.commandPool = _device.queueMgr.GetQueueFromType(_queueType).GetCommandPool(_poolIndex);
		commandBufferAllocInfo.level = _level;
		commandBufferAllocInfo.commandBufferCount = 1u;

		SA_VK_ASSERT(vkAllocateCommandBuffers(_device, &commandBufferAllocInfo, &result.mHandle),
			CreationFailed, Rendering, L"Failed to allocate command buffers!");

		return result;
	}

	std::vector<CommandBuffer> CommandBuffer::AllocateMultiple(const Device& _device, QueueType _queueType, uint32 _num, uint32 _poolIndex, VkCommandBufferLevel _level)
	{
		std::vector<CommandBuffer> result;
		result.resize(_num, CommandBuffer(_poolIndex, _queueType));

		std::vector<VkCommandBuffer> vkCommandBuffers;
		vkCommandBuffers.resize(_num);

		VkCommandBufferAllocateInfo commandBufferAllocInfo{};
		commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocInfo.pNext = nullptr;
		commandBufferAllocInfo.commandPool = _device.queueMgr.GetQueueFromType(_queueType).GetCommandPool(_poolIndex);
		commandBufferAllocInfo.level = _level;
		commandBufferAllocInfo.commandBufferCount = _num;

		SA_VK_ASSERT(vkAllocateCommandBuffers(_device, &commandBufferAllocInfo, vkCommandBuffers.data()),
			CreationFailed, Rendering, L"Failed to allocate command buffers!");

		for (uint32 i = 0; i < _num; ++i)
			result[i].mHandle = vkCommandBuffers[i];

		return result;
	}

	void CommandBuffer::Free(const Device& _device, CommandBuffer& _commandBuffer)
	{
		SA_ASSERT(_commandBuffer.mHandle != VK_NULL_HANDLE, InvalidParam, Rendering, L"Destroy a null command buffer!");

		vkFreeCommandBuffers(_device, _device.queueMgr.GetQueueFromType(_commandBuffer.mQueueType).GetCommandPool(_commandBuffer.mPoolIndex),
			1u, &_commandBuffer.mHandle);
		
		_commandBuffer.mHandle = VK_NULL_HANDLE;
	}
	
	void CommandBuffer::FreeMultiple(const Device& _device, std::vector<CommandBuffer>& _buffers)
	{
		const uint32 num = SizeOf(_buffers);

		std::vector<VkCommandBuffer> vkCommandBuffers;
		vkCommandBuffers.resize(num);

		for (uint32 i = 0; i < num; ++i)
		{
			vkCommandBuffers[i] = _buffers[i].mHandle;
			_buffers[i].mHandle = VK_NULL_HANDLE;
		}

		vkFreeCommandBuffers(_device, _device.queueMgr.GetQueueFromType(_buffers[0].mQueueType).GetCommandPool(_buffers[0].mPoolIndex),
			num, vkCommandBuffers.data());
	}


	CommandBuffer CommandBuffer::BeginSingleTimeCommands(const Device& _device, QueueType _queueType, uint32 _poolIndex)
	{
		CommandBuffer commandBuffer = Allocate(_device, _queueType, _poolIndex);

		// Start command buffer record.
		VkCommandBufferBeginInfo commandBeginInfo{};
		commandBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBeginInfo.pNext = nullptr;
		commandBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		commandBeginInfo.pInheritanceInfo = nullptr;

		vkBeginCommandBuffer(commandBuffer, &commandBeginInfo);

		return commandBuffer;
	}
	
	void CommandBuffer::EndSingleTimeCommands(const Device& _device, CommandBuffer& _commandBuffer)
	{
		// End command buffer record.
		vkEndCommandBuffer(_commandBuffer);

		// Submit commands.
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = nullptr;
		submitInfo.waitSemaphoreCount = 0u;
		submitInfo.pWaitSemaphores = nullptr;
		submitInfo.pWaitDstStageMask = nullptr;
		submitInfo.commandBufferCount = 1u;
		submitInfo.pCommandBuffers = &_commandBuffer.mHandle;
		submitInfo.signalSemaphoreCount = 0u;
		submitInfo.pSignalSemaphores = nullptr;

		VkQueue vkQueue = _device.queueMgr.GetQueueFromType(_commandBuffer.mQueueType).GetHandle(_commandBuffer.GetPoolIndex());

		vkQueueSubmit(vkQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(vkQueue);


		// Free command buffer.
		Free(_device, _commandBuffer);
	}


	CommandBuffer::operator VkCommandBuffer() const noexcept
	{
		return mHandle;
	}
}
