// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_RENDERING_VK_MESH_GUARD
#define SAPPHIRE_RENDERING_VK_MESH_GUARD

#include <Rendering/Framework/Model/IMesh.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkDevice;

	class VkMesh : public IMesh
	{
		VkBuffer mVertexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory mVertexBufferMemory = VK_NULL_HANDLE;

	public:
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Create(const IRenderInstance& _instance, const std::vector<Vertex>& _vertices) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Destroy(const IRenderInstance& _instance) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API operator VkBuffer() const noexcept;
	};
}

#endif

#endif // GUARD
