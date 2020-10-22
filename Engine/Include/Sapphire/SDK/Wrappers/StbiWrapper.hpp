// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_STBI_WRAPPER_GUARD
#define SAPPHIRE_SDK_STBI_WRAPPER_GUARD

#include <string>

#include <Core/Types/Int.hpp>

#include <SDK/Config.hpp>

namespace Sa
{
	class TextureAsset;
	class CubemapAsset;

	struct RawTexture;
	struct CubemapAssetImportInfos;

	class StbiWrapper
	{
		static void GenerateMipMaps(RawTexture& _rawData);

	public:
		static const uint32 bitSize;

		static void FlipVertically(RawTexture& _rawData);

		static void* Allocate(uint64 _dataSize);
		static void Free(void* _data);

		// Import single texture.
		static bool Import(const std::string& _resourcePath, TextureAsset& _result);

		// Import cubemap.
		static bool Import(const CubemapAssetImportInfos& _importInfos, CubemapAsset& _result);
	};
}

#endif // GUARD
