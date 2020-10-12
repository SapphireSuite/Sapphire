// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_ASSET_IMPORT_RESULT_GUARD
#define SAPPHIRE_SDK_ASSET_IMPORT_RESULT_GUARD

#include <vector>

#include <SDK/Config.hpp>

namespace Sa
{
	class IAsset;

	class IAssetImportResult : public std::vector<IAsset*>
	{
	public:
		IAssetImportResult() = default;
		IAssetImportResult(IAssetImportResult&&) = default;
		IAssetImportResult(const IAssetImportResult&) = delete;

		SA_ENGINE_API ~IAssetImportResult();

		IAssetImportResult& operator=(IAssetImportResult&&) = default;
		IAssetImportResult& operator=(const IAssetImportResult&) = delete;
	};
}

#endif // GUARD
