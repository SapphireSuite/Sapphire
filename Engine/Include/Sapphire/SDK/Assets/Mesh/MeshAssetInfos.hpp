// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MESH_ASSET_INFOS_GUARD
#define SAPPHIRE_SDK_MESH_ASSET_INFOS_GUARD

#include <Rendering/Framework/Primitives/Mesh/Vertex.hpp>

namespace Sa
{
	struct MeshRawData
	{
		std::vector<Vertex> vertices;
		std::vector<uint32> indices;
	};
}

#endif // GUARD
