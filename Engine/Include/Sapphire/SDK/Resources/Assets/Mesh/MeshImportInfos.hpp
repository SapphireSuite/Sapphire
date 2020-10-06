// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MESH_IMPORT_INFOS_GUARD
#define SAPPHIRE_SDK_MESH_IMPORT_INFOS_GUARD

#include <SDK/Resources/Assets/IAssetImportInfos.hpp>

#include <Rendering/Framework/Primitives/Mesh/Vertex.hpp>

namespace Sa
{
	struct MeshImportInfos : public IAssetImportInfos
	{
		std::vector<std::string> outFilePaths;

		SA_ENGINE_API MeshImportInfos() = default;
		SA_ENGINE_API MeshImportInfos(std::vector<std::string> && _outFilePaths, bool _bKeepLoaded = true) noexcept;
		SA_ENGINE_API MeshImportInfos(const std::vector<std::string>& _outFilePaths, bool _bKeepLoaded = true) noexcept;
	};

	struct MeshCreateInfos : public IAssetCreateInfos
	{
		std::vector<Vertex> vertices;
		std::vector<uint32> indices;
	};
}

#endif // GUARD
