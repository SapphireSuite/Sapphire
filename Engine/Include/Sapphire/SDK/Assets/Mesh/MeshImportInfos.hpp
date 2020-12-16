// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MESH_IMPORT_INFOS_GUARD
#define SAPPHIRE_SDK_MESH_IMPORT_INFOS_GUARD

#include <SDK/Assets/IAssetImportInfos.hpp>

namespace Sa
{
	class MeshImportInfos : public IAssetImportInfos
	{
	public:
		// Mesh index in imported file.
		uint32 index = 0u;

		std::shared_ptr<VertexLayout> layout = VertexLayout::Make(VertexComp::Default);
	};
}

#endif // GUARD
