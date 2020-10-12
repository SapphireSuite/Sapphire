// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_TINY_GLTF_WRAPPER_GUARD
#define SAPPHIRE_SDK_TINY_GLTF_WRAPPER_GUARD

#include <string>

namespace Sa
{
	class AssetManager;
	class IAssetImportResult;

	class TinyGLTFWrapper
	{
	public:
		static bool Import(const std::string& _resourcePath, AssetManager& _assetMgr, IAssetImportResult& _result);
	};
}

#endif // GUARD
