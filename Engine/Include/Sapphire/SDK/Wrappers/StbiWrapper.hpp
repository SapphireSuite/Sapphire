// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_STBI_WRAPPER_GUARD
#define SAPPHIRE_SDK_STBI_WRAPPER_GUARD

#include <string>

#include <Core/Types/Int.hpp>

#include <SDK/Config.hpp>

namespace Sa
{
	struct RawTexture;
	class TextureAsset;
	class TextureImportInfos;

	struct RawCubemap;
	struct CubemapAssetImportInfos;
	class CubemapAsset;
	class CubemapImportInfos;

	class StbiWrapper
	{
		static void GenerateMipMaps(RawTexture& _rawData);
		static void GenerateMipMaps(RawCubemap& _rawData);
		static void GenerateMipMaps(uint32 _width, uint32 _height, uint32 _mipLevels, char*& _data, uint32 _channelNum);

	public:
		static const uint32 bitSize;

		static void FlipVertically(RawTexture& _rawData);

		static void* Allocate(uint64 _dataSize);
		static void Free(void* _data);

		// Import single texture.
		static bool Import(const std::string& _resourcePath, TextureAsset& _result, const TextureImportInfos& _importInfos);

		// Import cubemap.
		static bool Import(const CubemapAssetImportInfos& _assetImportInfos, CubemapAsset& _result, const CubemapImportInfos& _importInfos);
	};
}

#endif // GUARD
