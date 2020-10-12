// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_STBI_WRAPPER_GUARD
#define SAPPHIRE_SDK_STBI_WRAPPER_GUARD

#include <string>

#include <Core/Types/Int.hpp>
#include <SDK/Assets/Texture/TextureChannel.hpp>

namespace Sa
{
	class AssetManager;
	class IAssetImportResult;

	class StbiWrapper
	{
	public:
		static uint32 GetDataSize(uint32 _width, uint32 _height, TextureChannel _channel);

		static void FlipVertically(void* _data, uint32 _width, uint32 _height, TextureChannel _channel);

		static void* Allocate(uint32 _dataSize);
		static void Free(void* _data);

		static bool Import(const std::string& _resourcePath, AssetManager& _assetMgr, IAssetImportResult& _result);
	};
}

#endif // GUARD
