// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

namespace Sa::Vk
{
	bool Buffer::IsValid() const noexcept
	{
		return mHandle != VK_NULL_HANDLE && mDeviceMemory != VK_NULL_HANDLE;
	}

	void Buffer::Create_Internal(const Device& _device,
		uint64 _size, VkBufferUsageFlags _usage,
		VkMemoryPropertyFlags _properties,
		const void* _data)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.pNext = nullptr;
		bufferInfo.flags = 0u;
		bufferInfo.size = _size;
		bufferInfo.usage = _usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferInfo.queueFamilyIndexCount = 0u;
		bufferInfo.pQueueFamilyIndices = nullptr;

		SA_VK_ASSERT(vkCreateBuffer(_device, &bufferInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create vertex buffer!");


		// === Memory ===
		VkMemoryRequirements memRequirements{};
		vkGetBufferMemoryRequirements(_device, mHandle, &memRequirements);

		uint32 memoryTypeIndex = FindMemoryType(_device, memRequirements.memoryTypeBits, _properties);

		VkMemoryAllocateInfo memoryAllocInfo{};
		memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocInfo.pNext = nullptr;
		memoryAllocInfo.allocationSize = memRequirements.size;
		memoryAllocInfo.memoryTypeIndex = memoryTypeIndex;

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

	void Buffer::Create(const Device& _device,
		uint64 _size, VkBufferUsageFlags _usage,
		VkMemoryPropertyFlags _properties,
		const void* _data)
	{
		// Use staging buffer for device local buffer.
		if (_properties == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		{
			// Create temp staging buffer.
			Buffer stagingBuffer;
			stagingBuffer.Create_Internal(_device, _size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _data);


			// Create saved buffer (device local only).
			Create_Internal(_device, _size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | _usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);


			// Use staging buffer to transfer mapped memory.
			Buffer::Copy(_device, stagingBuffer, *this, _size);


			// Destroy staging buffer.
			stagingBuffer.Destroy(_device);
		}
		else
			Create_Internal(_device, _size, _usage, _properties, _data);
	}

	void Buffer::Destroy(const Device& _device)
	{
		SA_ASSERT(IsValid(), InvalidParam, Rendering, L"Destroy a null Buffer!")

		vkDestroyBuffer(_device, mHandle, nullptr);
		vkFreeMemory(_device, mDeviceMemory, nullptr); // Should be freed after destroying buffer.

		mHandle = VK_NULL_HANDLE;
		mDeviceMemory = VK_NULL_HANDLE;
	}


	void Buffer::UpdateData(const Device& _device, const void* _data, uint32 _size, uint32 _offset)
	{
		SA_ASSERT(_data, Nullptr, Rendering, L"Update nullptr data!");

		void* bufferData = nullptr;

		vkMapMemory(_device, mDeviceMemory, _offset, _size, 0, &bufferData);

		memcpy(bufferData, _data, _size);

		vkUnmapMemory(_device, mDeviceMemory);
	}

	VkDescriptorBufferInfo Buffer::CreateDescriptorBufferInfo(uint32 _size) const noexcept
	{
		VkDescriptorBufferInfo descInfos{};
		descInfos.buffer = mHandle;
		descInfos.offset = 0u;
		descInfos.range = _size;

		return descInfos;
	}

	VkWriteDescriptorSet Buffer::CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, VkDescriptorType _type, uint32 _binding, uint32 _arrayElem) noexcept
	{
		VkWriteDescriptorSet writeDesc{};
		writeDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDesc.pNext = nullptr;
		writeDesc.dstSet = _descriptorSet;
		writeDesc.dstBinding = _binding;
		writeDesc.dstArrayElement = _arrayElem;
		writeDesc.descriptorCount = 1u;
		writeDesc.descriptorType = _type;
		writeDesc.pImageInfo = nullptr; // Will be set in pipeline.
		writeDesc.pBufferInfo = nullptr; // Will be set in pipeline.
		writeDesc.pTexelBufferView = nullptr; // Will be set in pipeline.

		return writeDesc;
	}


	void Buffer::Copy(const Device& _device, const Buffer& _src, const Buffer& _dst, uint64 _size)
	{
		CommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(_device, QueueType::Transfer);

		// Add copy command.
		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0u;
		copyRegion.dstOffset = 0u;
		copyRegion.size = _size;

		vkCmdCopyBuffer(commandBuffer, _src, _dst, 1, &copyRegion);

		CommandBuffer::EndSingleTimeCommands(_device, commandBuffer);
	}


	uint32 Buffer::FindMemoryType(const Device& _device, uint32 _typeFilter, VkMemoryPropertyFlags _properties)
	{
		const VkPhysicalDeviceMemoryProperties& memProperties = _device.GetMemoryProperties();

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((_typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & _properties) == _properties)
				return i;
		}

		SA_ASSERT(false, NotSupported, Rendering, L"Memory type requiered not supported!");

		return uint32(-1);
	}


	Buffer::operator VkBuffer() const noexcept
	{
		return mHandle;
	}

	Buffer::operator VkDeviceMemory() const noexcept
	{
		return mDeviceMemory;
	}
}
