// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Mesh/VkMesh.hpp>

#include <Core/Types/Variadics/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkDevice.hpp>
#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkMesh::Create(const IRenderInstance& _instance,
		const std::vector<Vertex>& _vertices,
		const std::vector<uint32>& _indices)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();


		// Create Vertex buffer.
		mVertexBuffer.Create(device, sizeof(Vertex) * SizeOf(_vertices),
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			_vertices.data());


		// Create Index buffer.
		mIndicesSize = SizeOf(_indices);
		mIndexBuffer.Create(device, sizeof(uint32) * mIndicesSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			_indices.data());
	}

	void VkMesh::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		mVertexBuffer.Destroy(device);
		mIndexBuffer.Destroy(device);
	}

	void VkMesh::Draw(const IRenderFrame& _frame) const
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
