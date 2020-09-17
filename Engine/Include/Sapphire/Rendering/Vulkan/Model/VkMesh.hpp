// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_MESH_GUARD
#define SAPPHIRE_RENDERING_VK_MESH_GUARD

#include <Rendering/Framework/Model/IMesh.hpp>
#include <Rendering/Vulkan/Buffer/VkBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkDevice;

	class VkMesh : public IMesh
	{
		VkBuffer mVertexBuffer;

	public:
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Create(const IRenderInstance& _instance, const std::vector<Vertex>& _vertices) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Destroy(const IRenderInstance& _instance) override final;

		// TODO: Remove later.
		SA_ENGINE_API operator const VkBuffer& () const noexcept;
	};
}

#endif

#endif // GUARD
