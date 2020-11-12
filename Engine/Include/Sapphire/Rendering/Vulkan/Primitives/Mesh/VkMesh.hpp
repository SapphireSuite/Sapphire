// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_MESH_GUARD
#define SAPPHIRE_RENDERING_VK_MESH_GUARD

#include <Rendering/Framework/Primitives/Mesh/IMesh.hpp>

#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class Mesh : public IMesh
	{
		Buffer mVertexBuffer;
		Buffer mIndexBuffer;

		uint32 mIndicesSize = uint32(-1);

	public:
		void Create(const IRenderInstance& _instance, const RawMesh& _rawMesh) override final;
		void Destroy(const IRenderInstance& _instance) override final;
	};
}

#endif

#endif // GUARD
