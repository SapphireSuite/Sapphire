// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/AssetImportResult.hpp>

#include <SDK/Assets/IAsset.hpp>

namespace Sa
{
	IAssetImportResult::~IAssetImportResult()
	{
		for (auto it = begin(); it != end(); ++it)
			delete* it;

		clear();
	}
}