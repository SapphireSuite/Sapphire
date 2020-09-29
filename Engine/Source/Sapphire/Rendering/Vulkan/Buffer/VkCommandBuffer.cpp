// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>

#include <Rendering/Vulkan/System/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkCommandBuffer::BeginSingleTimeCommands_Internal(const VkDevice& _device, const VkQueue& _queue)
	{
		// Allocate command buffer.
		const VkCommandBufferAllocateInfo bufferAllocInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,					// sType.
			nullptr,														// pNext.
			_queue.GetCommandPool(),										// commandPool.
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,								// level.
			1,																// commandBufferCount.
		};

		vkAllocateCommandBuffers(_device, &bufferAllocInfo, &mHandle);


		// Start command buffer record.
		const VkCommandBufferBeginInfo commandBeginInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,					// sType.
			nullptr,														// pNext.
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,					// flags.
			nullptr															// pInheritanceInfo.
		};

		vkBeginCommandBuffer(mHandle, &commandBeginInfo);
	}

	void VkCommandBuffer::EndSingleTimeCommands_Internal(const VkDevice& _device, const VkQueue& _queue)
	{
		// End command buffer record.
		vkEndCommandBuffer(mHandle);


		// Submit commands.
		const VkSubmitInfo submitInfo
		{
			VK_STRUCTURE_TYPE_SUBMIT_INFO,									// sType.
			nullptr,														// pNext.
			0,																// waitSemaphoreCount.
			nullptr,														// pWaitSemaphores.
			nullptr,														// pWaitDstStageMask.
			1,																// commandBufferCount.
			&mHandle,														// pCommandBuffers.
			0,																// signalSemaphoreCount.
			nullptr															// pSignalSemaphores.
		};

		vkQueueSubmit(_queue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(_queue);


		// Free command buffer.
		vkFreeCommandBuffers(_device, _queue.GetCommandPool(), 1, &mHandle);
	}

	VkCommandBuffer VkCommandBuffer::BeginSingleTimeCommands(const VkDevice& _device, const VkQueue& _queue)
	{
		VkCommandBuffer commandBuffer;

		commandBuffer.BeginSingleTimeCommands_Internal(_device, _queue);

		return commandBuffer;
	}

	void VkCommandBuffer::EndSingleTimeCommands(const VkDevice& _device, VkCommandBuffer _commandBuffer, const VkQueue& _queue)
	{
		_commandBuffer.EndSingleTimeCommands_Internal(_device, _queue);
	}

	VkCommandBuffer::operator ::VkCommandBuffer() const
	{
		return mHandle;
	}
}

#endif
