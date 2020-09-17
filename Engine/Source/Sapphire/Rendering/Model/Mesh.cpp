// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Model/Mesh.hpp>

#include <Rendering/Model/Vertex.hpp>

#include <Rendering/Vulkan/VkMacro.hpp>
#include <Rendering/Vulkan/Primitives/VkDevice.hpp>

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

		SA_ASSERT(false, NotSupported, Rendering, L"Memory type requiered not supported!")

		return -1;
	}

	void Mesh::Create(const VkDevice& _device, const std::vector<Vertex>& _vertices)
	{
		const VkBufferCreateInfo bufferInfo
		{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,						// sType.
			nullptr,													// pNext.
			0,															// flags.
			sizeof(Vertex) * _vertices.size(),							// size.
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,							// usage.
			VK_SHARING_MODE_EXCLUSIVE,									// sharingMode.
			0,															// queueFamilyIndexCount.
			nullptr,													// pQueueFamilyIndices.
		};

		SA_VK_ASSERT(vkCreateBuffer(_device, &bufferInfo, nullptr, &mVertexBuffer),
			CreationFailed, Rendering, L"Failed to create vertex buffer!");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(_device, mVertexBuffer, &memRequirements);

		uint32 memoryTypeIndex = FindMemoryType(_device, memRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		const VkMemoryAllocateInfo memoryAllocInfo
		{
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,						// sType.
			nullptr,													// pNext.
			memRequirements.size,										// allocationSize.
			memoryTypeIndex												// memoryTypeIndex.
		};

		SA_VK_ASSERT(vkAllocateMemory(_device, &memoryAllocInfo, nullptr, &mVertexBufferMemory),
			MemoryAllocationFailed, Rendering, L"Failed to allocate vertex buffer memory!");

		vkBindBufferMemory(_device, mVertexBuffer, mVertexBufferMemory, 0);


		// Map memory.
		void* data;
		vkMapMemory(_device, mVertexBufferMemory, 0, bufferInfo.size, 0, &data);
		
		memcpy(data, _vertices.data(), static_cast<uint32>(bufferInfo.size));
		
		vkUnmapMemory(_device, mVertexBufferMemory);

	}

	void Mesh::Destroy(const VkDevice& _device)
	{
		vkDestroyBuffer(_device, mVertexBuffer, nullptr);
		vkFreeMemory(_device, mVertexBufferMemory, nullptr); // Should be freed after destroying buffer.

		mVertexBuffer = VK_NULL_HANDLE;
		mVertexBufferMemory = VK_NULL_HANDLE;
	}

	Mesh::operator VkBuffer() const noexcept
	{
		return mVertexBuffer;
	}
}
