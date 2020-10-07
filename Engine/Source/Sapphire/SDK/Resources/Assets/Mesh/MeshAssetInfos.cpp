// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/Mesh/MeshAssetInfos.hpp>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	MeshImportInfos::MeshImportInfos(std::vector<std::string>&& _outFilePaths, bool _bKeepLoaded) noexcept :
		IAssetImportInfos(_bKeepLoaded),
		outFilePaths{ Move(_outFilePaths) }
	{
	}
	MeshImportInfos::MeshImportInfos(const std::vector<std::string>& _outFilePaths, bool _bKeepLoaded) noexcept :
		IAssetImportInfos(_bKeepLoaded),
		outFilePaths{ _outFilePaths }
	{
	}
}
