// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RAW_MESH_GUARD
#define SAPPHIRE_RENDERING_RAW_MESH_GUARD

#include <vector>

#include <Rendering/Framework/Primitives/Mesh/Vertex.hpp>

namespace Sa
{
	struct RawMesh
	{
		std::vector<Vertex> vertices;
		std::vector<uint32> indices;
	};
}

#endif // GUARD
