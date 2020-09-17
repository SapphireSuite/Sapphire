// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_RENDERING_MESH_GUARD
#define SAPPHIRE_RENDERING_MESH_GUARD

#include <vector>

#include <vulkan/vulkan.h>

#include <Rendering/Config.hpp>

namespace Sa
{
	class Vertex;
	class VkDevice;

	class Mesh
	{
		VkBuffer mVertexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory mVertexBufferMemory = VK_NULL_HANDLE;

	public:
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Create(const VkDevice& _device, const std::vector<Vertex>& _vertices);
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Destroy(const VkDevice& _device);

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API operator VkBuffer() const noexcept;
	};
}

#endif // GUARD
