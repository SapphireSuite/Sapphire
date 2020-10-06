// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/Material/MaterialImportInfos.hpp>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	MaterialImportInfos::MaterialImportInfos(std::vector<std::string>&& _outFilePaths, bool _bKeepLoaded) noexcept :
		IAssetImportInfos(_bKeepLoaded),
		outFilePaths{ Move(_outFilePaths) }
	{
	}
	MaterialImportInfos::MaterialImportInfos(const std::vector<std::string>& _outFilePaths, bool _bKeepLoaded) noexcept :
		IAssetImportInfos(_bKeepLoaded),
		outFilePaths{ _outFilePaths }
	{
	}
}
