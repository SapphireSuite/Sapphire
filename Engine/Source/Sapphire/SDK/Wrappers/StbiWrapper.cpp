// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Wrappers/StbiWrapper.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Collections/Debug>

#include <Core/Algorithms/Move.hpp>

#include <SDK/Assets/AssetManager.hpp>

namespace Sa
{
	uint32 StbiWrapper::GetDataSize(uint32 _width, uint32 _height, TextureChannel _channel)
	{
		return _width * _height * static_cast<int32>(_channel) * sizeof(stbi_uc);
	}
	
	void StbiWrapper::FlipVertically(void* _data, uint32 _width, uint32 _height, TextureChannel _channel)
	{
		SA_ASSERT(_data, Nullptr, SDK_Asset, L"Flip nullptr data!");

		stbi__vertical_flip(_data, static_cast<int32>(_width), static_cast<int32>(_height), static_cast<int32>(_channel) * sizeof(stbi_uc));
	}

	void* StbiWrapper::Allocate(uint32 _dataSize)
	{
		return stbi__malloc(_dataSize);
	}
	
	void StbiWrapper::Free(void* _data)
	{
		SA_ASSERT(_data, Nullptr, SDK_Asset, L"Free nullptr data!");

		stbi_image_free(_data);
	}

	bool StbiWrapper::Import(const std::string& _resourcePath, AssetManager& _assetMgr, IAssetImportResult& _result)
	{
		// TODO: REMOVE LATER.
		stbi_set_flip_vertically_on_load(true);

		TextureRawData rawData;

		// Load texture.
		rawData.data = reinterpret_cast<char*>(stbi_load(_resourcePath.c_str(),
			reinterpret_cast<int32*>(&rawData.width),
			reinterpret_cast<int32*>(&rawData.height),
			reinterpret_cast<int32*>(&rawData.channel),
			static_cast<int32>(rawData.channel)
		));

		if (!rawData.data)
		{
			SA_LOG("Failed to load texture image!", Error, SDK_Asset);
			return false;
		}

		_result.emplace_back(new TextureAsset(_assetMgr.textureMgr, Move(rawData)));

		return true;
	}
}
