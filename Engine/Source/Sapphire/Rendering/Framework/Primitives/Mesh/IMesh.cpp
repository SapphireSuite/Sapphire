// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Mesh/IMesh.hpp>

#include <Rendering/Config.hpp>

#include <Rendering/Vulkan/Primitives/Mesh/VkMesh.hpp>

namespace Sa
{
	std::shared_ptr<VertexLayout> IMesh::GetLayout() const noexcept
	{
		return mLayout;
	}

	IMesh* IMesh::CreateInstance(const IRenderInstance& _instance, const RawMesh& _rawMesh)
	{
#if SA_RENDERING_API == SA_VULKAN

		IMesh* result = new VkMesh;

#endif
		result->mLayout = _rawMesh.GetLayout();
		result->Create(_instance, _rawMesh);

		return result;
	}
}