// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Mesh/IMesh.hpp>

namespace Sa
{
	std::shared_ptr<VertexLayout> IMesh::GetLayout() const noexcept
	{
		return mLayout;
	}
}
