// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Wrappers/StbiWrapper.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#include <Collections/Debug>

#include <Core/Algorithms/Move.hpp>

#include <Rendering/Framework/Primitives/Texture/Mipmap.hpp>

#include <SDK/Assets/AssetManager.hpp>

namespace Sa
{
	const uint32 StbiWrapper::bitSize = sizeof(stbi_uc);
	
	void StbiWrapper::FlipVertically(RawTexture& _rawData)
	{
		SA_ASSERT(_rawData.data, Nullptr, SDK_Asset, L"Flip nullptr data!");

		MipMapInfos* infos = new MipMapInfos[_rawData.mipLevels];

		Mipmap::ComputeTotalSize(_rawData.width, _rawData.height, _rawData.mipLevels, infos);

		uint32 offset = 0u;
		int32 channelSize = static_cast<int32>(_rawData.channel);

		// Flip each mipmap.
		for (uint32 i = 0; i < _rawData.mipLevels; ++i)
		{
			stbi__vertical_flip(_rawData.data + offset,
				static_cast<int32>(infos[i].width),
				static_cast<int32>(infos[i].height),
				channelSize * bitSize);

			offset += infos[i].width * infos[i].height * channelSize * bitSize;
		}

		delete[] infos;
	}

	void* StbiWrapper::Allocate(uint64 _dataSize)
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

		GenerateMipMaps(rawData);

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

		uint64 size = rawData.GetMainSize(false) * bitSize;
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

	void StbiWrapper::GenerateMipMaps(RawTexture& _rawData)
	{
		_rawData.mipLevels = Mipmap::ComputeLevels(_rawData.width, _rawData.height);

		// Can't generate mipMapLevels.
		if (_rawData.mipLevels <= 1u)
		{
			_rawData.mipLevels = 1u;
			return;
		}

		MipMapInfos* mipmapInfos = new MipMapInfos[_rawData.mipLevels];

		uint32 channelSize = static_cast<uint32>(_rawData.channel);
		uint64 totalSize = Mipmap::ComputeTotalSize(_rawData.width, _rawData.height, _rawData.mipLevels, mipmapInfos);

		char* original = _rawData.data;
		_rawData.data = reinterpret_cast<char*>(Allocate(totalSize * bitSize * channelSize));

		// Move original data.
		MemMove(original, _rawData.data, _rawData.width * _rawData.height * bitSize * channelSize);
		Free(original);

		unsigned char* src = reinterpret_cast<unsigned char*>(_rawData.data);

		for (uint32 i = 1u; i < _rawData.mipLevels; ++i)
		{
			uint32 offset = mipmapInfos[i - 1].width * mipmapInfos[i - 1].height * bitSize * channelSize;
			unsigned char* dst = src + offset;

			bool res = stbir_resize_uint8(src, mipmapInfos[i - 1].width, mipmapInfos[i - 1].height, 0,
				dst, mipmapInfos[i].width, mipmapInfos[i].height, 0,
				channelSize);

			SA_ASSERT(res, CreationFailed, SDK_Assert, L"Mip map creation failed!");

			src = dst;
		}

		delete[] mipmapInfos;
	}
}
