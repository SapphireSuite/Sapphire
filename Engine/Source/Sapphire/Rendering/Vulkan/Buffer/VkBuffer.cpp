// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffer/VkBuffer.hpp>

#include <Rendering/Vulkan/VkMacro.hpp>
#include <Rendering/Vulkan/Primitives/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	uint32 FindMemoryType(const VkDevice& _device, uint32 _typeFilter, VkMemoryPropertyFlags _properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(_device, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((_typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & _properties) == _properties)
				return i;
		}

		SA_ASSERT(false, NotSupported, Rendering, L"Memory type requiered not supported!");

		return uint32(-1);
	}

	void VkBuffer::Create(const VkDevice& _device, uint32 _size, VkBufferUsageFlags _usage, VkMemoryPropertyFlags _properties)
	{
		const VkBufferCreateInfo bufferInfo
		{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,						// sType.
			nullptr,													// pNext.
			0,															// flags.
			_size,														// size.
			_usage,														// usage.
			VK_SHARING_MODE_EXCLUSIVE,									// sharingMode.
			VkQueueFamilyIndices::familyNum,							// queueFamilyIndexCount.
			_device.GetQueueFamilyIndices().GetFamilies(),				// pQueueFamilyIndices.
		};

		SA_VK_ASSERT(vkCreateBuffer(_device, &bufferInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create vertex buffer!");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(_device, mHandle, &memRequirements);

		uint32 memoryTypeIndex = FindMemoryType(_device, memRequirements.memoryTypeBits, _properties);

		const VkMemoryAllocateInfo memoryAllocInfo
		{
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,						// sType.
			nullptr,													// pNext.
			memRequirements.size,										// allocationSize.
			memoryTypeIndex												// memoryTypeIndex.
		};

		SA_VK_ASSERT(vkAllocateMemory(_device, &memoryAllocInfo, nullptr, &mDeviceMemory),
			MemoryAllocationFailed, Rendering, L"Failed to allocate vertex buffer memory!");

		vkBindBufferMemory(_device, mHandle, mDeviceMemory, 0);
	}

	void VkBuffer::Destroy(const VkDevice& _device)
	{
		vkDestroyBuffer(_device, mHandle, nullptr);
		vkFreeMemory(_device, mDeviceMemory, nullptr); // Should be freed after destroying buffer.

		mHandle = VK_NULL_HANDLE;
		mDeviceMemory = VK_NULL_HANDLE;
	}

	void VkBuffer::Copy(const VkDevice& _device, const VkBuffer& _src, const VkBuffer& _dst, uint32 _size)
	{
		const VkQueue& transferQueue = _device.GetTransferQueue();


		// Allocate command buffer.
		const VkCommandBufferAllocateInfo bufferAllocInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,					// sType.
			nullptr,														// pNext.
			transferQueue.GetCommandPool(),									// commandPool.
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,								// level.
			1,																// commandBufferCount.
		};

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(_device, &bufferAllocInfo, &commandBuffer);


		// Start command buffer record.
		const VkCommandBufferBeginInfo commandBeginInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,					// sType.
			nullptr,														// pNext.
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,					// flags.
			nullptr															// pInheritanceInfo.
		};

		vkBeginCommandBuffer(commandBuffer, &commandBeginInfo);


		// Add copy command.
		const VkBufferCopy copyRegion
		{
			0,																// srcOffset.
			0,																// dstOffset.
			_size,															// size.
		};
		
		vkCmdCopyBuffer(commandBuffer, _src, _dst, 1, &copyRegion);


		// End command buffer record.
		vkEndCommandBuffer(commandBuffer);


		// Submit commands.
		const VkSubmitInfo submitInfo
		{
			VK_STRUCTURE_TYPE_SUBMIT_INFO,									// sType.
			nullptr,														// pNext.
			0,																// waitSemaphoreCount.
			nullptr,														// pWaitSemaphores.
			nullptr,														// pWaitDstStageMask.
			1,																// commandBufferCount.
			&commandBuffer,													// pCommandBuffers.
			0,																// signalSemaphoreCount.
			nullptr															// pSignalSemaphores.
		};

		vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(transferQueue);


		// Free command buffer.
		vkFreeCommandBuffers(_device, transferQueue.GetCommandPool(), 1, &commandBuffer);
	}

	VkBuffer::operator ::VkBuffer() const noexcept
	{
		return mHandle;
	}

	VkBuffer::operator VkDeviceMemory() const noexcept
	{
		return mDeviceMemory;
	}
}

#endif