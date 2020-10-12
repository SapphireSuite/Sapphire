// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_TINY_OBJ_WRAPPER_GUARD
#define SAPPHIRE_SDK_TINY_OBJ_WRAPPER_GUARD

#include <string>

namespace Sa
{
	class AssetManager;
	class IAssetImportResult;

	class TinyOBJWrapper
	{
	public:
		static bool ImportOBJ(const std::string& _resourcePath, AssetManager& _assetMgr, IAssetImportResult& _result);
		static bool ImportMTL(const std::string& _resourcePath, AssetManager& _assetMgr, IAssetImportResult& _result);
	};
}

#endif // GUARD
