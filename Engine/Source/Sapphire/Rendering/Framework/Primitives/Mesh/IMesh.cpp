// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Mesh/IMesh.hpp>

namespace Sa
{
	std::shared_ptr<VertexLayout> IMesh::GetLayout() const noexcept
	{
		return mLayout;
	}

	void IMesh::Create(const IRenderInstance& _instance, const RawMesh& _rawMesh)
	{
		(void)_instance;
		mLayout = _rawMesh.GetLayout();
	}
}
