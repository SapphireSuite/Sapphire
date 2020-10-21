// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Wrappers/StbiWrapper.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Collections/Debug>

#include <Core/Algorithms/Move.hpp>

#include <SDK/Assets/AssetManager.hpp>

namespace Sa
{
	const uint32 StbiWrapper::bitSize = sizeof(stbi_uc);
	
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

		// TODO: CLEAN LATER.
		RawTexture rawData;

		// Load texture.
		rawData.data = reinterpret_cast<char*>(stbi_load(_resourcePath.c_str(),
			reinterpret_cast<int32*>(&rawData.width),
			reinterpret_cast<int32*>(&rawData.height),
			reinterpret_cast<int32*>(&rawData.channel),
			4 // TODO: USE IMPORT INFOS channel.
		));

		// TODO: fix.
		rawData.channel = TextureChannel::RGBA;

		if (!rawData.data)
		{
			SA_LOG("Failed to load texture image!", Error, SDK_Asset);
			return false;
		}

		_result.emplace_back(new TextureAsset(_assetMgr.textureMgr, Move(rawData)));

		return true;
	}

	bool StbiWrapper::ImportCubemap(const CubemapAssetImportInfos& _importInfos, AssetManager& _assetMgr, IAssetImportResult& _result)
	{
		// TODO: REMOVE LATER.
		stbi_set_flip_vertically_on_load(true);


		// TODO: CLEAN LATER.
		RawTexture rawData;
		rawData.type = TextureType::Cubemap;

		char* data[6]{};

		// Load textures.
		for (uint32 i = 0u; i < 6u; ++i)
		{
			data[i] = reinterpret_cast<char*>(stbi_load(_importInfos.pathes[i].c_str(),
				reinterpret_cast<int32*>(&rawData.width),
				reinterpret_cast<int32*>(&rawData.height),
				reinterpret_cast<int32*>(&rawData.channel),
				4 // TODO: USE IMPORT INFOS channel.
			));

			if (!data[i])
			{
				SA_LOG("Failed to load texture image!", Error, SDK_Asset);
				return false;
			}
		}

		// TODO: fix.
		rawData.channel = TextureChannel::RGBA;

		uint32 size = rawData.GetSize(false) * bitSize;
		rawData.data = reinterpret_cast<char*>(Allocate(6u * size));

		// 
		for (uint32 i = 0u; i < 6u; ++i)
		{
			MemMove(data[i], rawData.data + i * size, size);

			Free(data[i]);
		}

		_result.emplace_back(new TextureAsset(_assetMgr.textureMgr, Move(rawData)));

		return true;
	}
}
