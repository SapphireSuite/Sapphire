// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffer/VkBuffer.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkDevice.hpp>
#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	bool VkBuffer::IsValid() const noexcept
	{
		return mHandle != VK_NULL_HANDLE && mDeviceMemory != VK_NULL_HANDLE;
	}

	void VkBuffer::Create_Internal(const VkDevice& _device,
		uint32 _size, VkBufferUsageFlags _usage,
		VkMemoryPropertyFlags _properties,
		const void* _data)
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


		// Map memory.
		if (_data)
		{
			void* deviceData;
			vkMapMemory(_device, mDeviceMemory, 0, _size, 0, &deviceData);

			memcpy(deviceData, _data, _size);

			vkUnmapMemory(_device, mDeviceMemory);
		}
	}

	void VkBuffer::Create(const VkDevice& _device,
		uint32 _size, VkBufferUsageFlags _usage,
		VkMemoryPropertyFlags _properties,
		const void* _data)
	{
		// Use staging buffer for device local buffer.
		if (_properties == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		{
			// Create temp staging buffer.
			VkBuffer stagingBuffer;
			stagingBuffer.Create_Internal(_device, _size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _data);


			// Create saved buffer (device local only).
			Create_Internal(_device, _size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | _usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);


			// Use staging buffer to transfer mapped memory.
			VkBuffer::Copy(_device, stagingBuffer, *this, _size);


			// Destroy staging buffer.
			stagingBuffer.Destroy(_device);
		}
		else
			Create_Internal(_device, _size, _usage, _properties, _data);
	}

	void VkBuffer::Destroy(const VkDevice& _device)
	{
#if SA_DEBUG
		if (mHandle == VK_NULL_HANDLE || mDeviceMemory == VK_NULL_HANDLE)
			SA_LOG(L"Destroy a non-created (or already destroyed) buffer!", Warning, Rendering);
#endif

		vkDestroyBuffer(_device, mHandle, nullptr);
		vkFreeMemory(_device, mDeviceMemory, nullptr); // Should be freed after destroying buffer.

		mHandle = VK_NULL_HANDLE;
		mDeviceMemory = VK_NULL_HANDLE;
	}

	void VkBuffer::UpdateData(const VkDevice& _device, const void* _data, uint32 _size, uint32 _offset)
	{
		SA_ASSERT(_data, Nullptr, Rendering, L"Update nullptr data!");

		void* bufferData = nullptr;

		vkMapMemory(_device, mDeviceMemory, _offset, _size, 0, &bufferData);

		memcpy(bufferData, _data, _size);

		vkUnmapMemory(_device, mDeviceMemory);
	}

	VkDescriptorBufferInfo VkBuffer::CreateDescriptorBufferInfo(uint32 _size) const noexcept
	{
		return VkDescriptorBufferInfo
		{
			mHandle,											// buffer.
			0,													// offset.
			_size												// range.
		};
	}
	VkWriteDescriptorSet VkBuffer::CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, VkDescriptorType _type, uint32 _binding, uint32 _arrayElem) noexcept
	{
		return VkWriteDescriptorSet
		{
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,				// sType.
			nullptr,											// pNext.
			_descriptorSet,										// dstSet.
			_binding,											// dstBinding.
			_arrayElem,											// dstArrayElement.
			1,													// descriptorCount.
			_type,												// descriptorType.
			nullptr,											// pImageInfo.
			nullptr,											// pBufferInfo.				// Will be set in pipeline.
			nullptr												// pTexelBufferView.
		};
	}

	void VkBuffer::Copy(const VkDevice& _device, const VkBuffer& _src, const VkBuffer& _dst, uint32 _size)
	{
		VkCommandBuffer commandBuffer = VkCommandBuffer::BeginSingleTimeCommands(_device, _device.GetTransferQueue());


		// Add copy command.
		const VkBufferCopy copyRegion
		{
			0,																// srcOffset.
			0,																// dstOffset.
			_size,															// size.
		};
		
		vkCmdCopyBuffer(commandBuffer, _src, _dst, 1, &copyRegion);


		VkCommandBuffer::EndSingleTimeCommands(_device, commandBuffer, _device.GetTransferQueue());
	}

	uint32 VkBuffer::FindMemoryType(const VkDevice& _device, uint32 _typeFilter, VkMemoryPropertyFlags _properties)
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
