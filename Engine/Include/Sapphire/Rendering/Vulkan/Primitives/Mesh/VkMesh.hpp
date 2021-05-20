// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_MESH_GUARD
#define SAPPHIRE_RENDERING_VK_MESH_GUARD

#include <Rendering/Framework/Primitives/Mesh/IMesh.hpp>

#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class SA_ENGINE_API Mesh : public IMesh
	{
		Buffer mVertexBuffer;
		Buffer mIndexBuffer;

		uint32 mIndicesSize = ~uint32();

	public:
		void Create(const IRenderInstance& _instance, const RawMesh& _rawMesh) override final;
		void Destroy(const IRenderInstance& _instance) override final;

		void Draw(const RenderFrame& _frame, const MeshDrawInfos& _infos = MeshDrawInfos()) const override final;
	};
}

#endif

#endif // GUARD
