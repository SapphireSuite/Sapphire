// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_MESH_GUARD
#define SAPPHIRE_RENDERING_VK_MESH_GUARD

#include <Rendering/Framework/Primitives/Mesh/IMesh.hpp>

#include <Rendering/Vulkan/Buffer/VkBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkDevice;

	class VkMesh : public IMesh
	{
	public:// TODO: REMOVE LATER
		VkBuffer mVertexBuffer;
		VkBuffer mIndexBuffer;

		uint32 mIndicesSize = uint32(-1);

		void Create(const IRenderInstance& _instance, const RawMesh& _rawMesh) override final;

	public:
		void Destroy(const IRenderInstance& _instance) override final;

		void Draw(const IRenderFrame& _frame, const MeshDrawInfos& _infos) const override final;
	};
}

#endif

#endif // GUARD
