// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MESH_CREATE_INFOS_GUARD
#define SAPPHIRE_SDK_MESH_CREATE_INFOS_GUARD

#include <vector>

#include <Core/Types/Int.hpp>
#include <Rendering/Framework/Primitives/Pipeline/Vertex.hpp>

#include <Sdk/Asset/Loaders/MaterialCreateInfos.hpp>

namespace Sa
{
	struct MeshCreateInfos
	{
		std::vector<Vertex> vertices;
		std::vector<uint32> indices;

		MaterialCreateInfos matInfos;
	};
}

#endif // GUARD
