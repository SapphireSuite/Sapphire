// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Model/VkMesh.hpp>

#include <Rendering/Vulkan/VkMacro.hpp>
#include <Rendering/Vulkan/VkRenderInstance.hpp>
#include <Rendering/Vulkan/Primitives/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkMesh::Create(const IRenderInstance& _instance, const std::vector<Vertex>& _vertices)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		const uint32 bufferSize = sizeof(Vertex) * static_cast<uint32>(_vertices.size());

		// Create Staging buffer.
		VkBuffer stagingBuffer;
		stagingBuffer.Create(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


		// Map vertices memory.
		void* data;
		vkMapMemory(device, stagingBuffer, 0, bufferSize, 0, &data);

		memcpy(data, _vertices.data(), bufferSize);

		vkUnmapMemory(device, stagingBuffer);


		// Create vertex buffer (device local only).
		mVertexBuffer.Create(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);


		// Use staging buffer to transfer mapped memory.
		VkBuffer::Copy(device, stagingBuffer, mVertexBuffer, bufferSize);


		// Destroy staging buffer.
		stagingBuffer.Destroy(device);
	}

	void VkMesh::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		mVertexBuffer.Destroy(device);
	}


	VkMesh::operator const VkBuffer& () const noexcept
	{
		return mVertexBuffer;
	}
}

#endif
