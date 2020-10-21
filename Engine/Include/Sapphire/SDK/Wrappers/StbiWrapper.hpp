// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_STBI_WRAPPER_GUARD
#define SAPPHIRE_SDK_STBI_WRAPPER_GUARD

#include <string>

#include <Core/Types/Int.hpp>

#include <Rendering/Framework/Primitives/Texture/RawTexture.hpp>

#include <SDK/Assets/Importer/CubemapAssetImportInfos.hpp>

namespace Sa
{
	class AssetManager;
	class IAssetImportResult;

	class StbiWrapper
	{
		static void GenerateMipMaps(RawTexture& _rawData);

	public:
		static const uint32 bitSize;

		static void FlipVertically(RawTexture& _rawData);

		static void* Allocate(uint32 _dataSize);
		static void Free(void* _data);

		static bool Import(const std::string& _resourcePath, AssetManager& _assetMgr, IAssetImportResult& _result);
		static bool ImportCubemap(const CubemapAssetImportInfos& _importInfos, AssetManager& _assetMgr, IAssetImportResult& _result);
	};
}

#endif // GUARD
