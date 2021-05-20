// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_STBI_WRAPPER_GUARD
#define SAPPHIRE_SDK_STBI_WRAPPER_GUARD

#include <string>

#include <Maths/Space/Vector2.hpp>

#include <SDK/Config.hpp>

namespace Sa
{
	struct RawTexture;
	class TextureImportInfos;

	struct RawCubemap;
	//class CubemapImportInfos;

	class SA_ENGINE_API StbiWrapper
	{
		static void GenerateMipMaps(RawTexture& _rawData);
		static void GenerateMipMaps(RawCubemap& _rawData);
		static void GenerateMipMaps(const Vec2ui& _extent, uint32 _mipLevels, std::vector<char>& _data, uint32 _channelNum, uint32 _layerNum);

	public:
		static const uint32 bitSize;

		static void FlipVertically(RawTexture& _rawData);

		static void* Allocate(uint64 _dataSize);
		static void Free(void* _data);

		static bool Import(const std::string& _resourcePath, RawTexture& _outTexture, const TextureImportInfos& _importInfos);
		static bool Import(const std::string& _resourcePath, RawCubemap& _outCubemap, const TextureImportInfos& _importInfos);
	};
}

#endif // GUARD
