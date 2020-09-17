// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Model/VkMesh.hpp>

#include <Rendering/Vulkan/VkMacro.hpp>
#include <Rendering/Vulkan/VkRenderInstance.hpp>
#include <Rendering/Vulkan/Primitives/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void CreateBuffer(const VkDevice& _device, VkBuffer& _buffer, VkBufferUsageFlags _usage, uint32 _bufferSize, const void* _data)
	{
		// Create temp staging buffer.
		VkBuffer stagingBuffer;
		stagingBuffer.Create(_device, _bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


		// Map vertices memory.
		void* data;
		vkMapMemory(_device, stagingBuffer, 0, _bufferSize, 0, &data);

		memcpy(data, _data, _bufferSize);

		vkUnmapMemory(_device, stagingBuffer);


		// Create saved buffer (device local only).
		_buffer.Create(_device, _bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | _usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);


		// Use staging buffer to transfer mapped memory.
		VkBuffer::Copy(_device, stagingBuffer, _buffer, _bufferSize);


		// Destroy staging buffer.
		stagingBuffer.Destroy(_device);
	}

	void VkMesh::Create(const IRenderInstance& _instance,
		const std::vector<Vertex>& _vertices,
		const std::vector<uint32>& _indices)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		// Create Vertex buffer.
		CreateBuffer(device, mVertexBuffer, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, sizeof(Vertex) * static_cast<uint32>(_vertices.size()), _vertices.data());

		// Create Index buffer.
		mIndicesSize = static_cast<uint32>(_indices.size());
		CreateBuffer(device, mIndexBuffer, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, sizeof(uint32) * mIndicesSize, _indices.data());
	}

	void VkMesh::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		mVertexBuffer.Destroy(device);
		mIndexBuffer.Destroy(device);
	}

	void VkMesh::Draw(const IRenderFrame& _frame)
	{
		const VkRenderFrame& vkFrame = _frame.As<VkRenderFrame>();

		VkDeviceSize offsets[] = { 0 };
		::VkBuffer vkHandleVertexBuffer = mVertexBuffer;
		vkCmdBindVertexBuffers(vkFrame.graphicsCommandBuffer, 0, 1, &vkHandleVertexBuffer, offsets);

		vkCmdBindIndexBuffer(vkFrame.graphicsCommandBuffer, mIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(vkFrame.graphicsCommandBuffer, mIndicesSize, 1, 0, 0, 0);
	}
}

#endif
