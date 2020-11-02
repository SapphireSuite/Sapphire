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
#include <SDK/Assets/Texture/TextureImportInfos.hpp>
#include <SDK/Assets/Texture/CubemapImportInfos.hpp>

namespace Sa
{
	const uint32 StbiWrapper::bitSize = sizeof(stbi_uc);
	
	void StbiWrapper::FlipVertically(RawTexture& _rawData)
	{
		SA_ASSERT(_rawData.data, Nullptr, SDK_Asset, L"Flip nullptr data!");

		MipMapInfos* infos = new MipMapInfos[_rawData.mipLevels];

		Mipmap::ComputeTotalSize(_rawData.width, _rawData.height, _rawData.mipLevels, infos);

		uint32 offset = 0u;

		// Flip each mipmap.
		for (uint32 i = 0; i < _rawData.mipLevels; ++i)
		{
			stbi__vertical_flip(_rawData.data + offset,
				static_cast<int32>(infos[i].width),
				static_cast<int32>(infos[i].height),
				_rawData.channelNum * bitSize);

			offset += infos[i].width * infos[i].height * _rawData.channelNum * bitSize;
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

	bool StbiWrapper::Import(const std::string& _resourcePath, TextureAsset& _result, const TextureImportInfos& _importInfos)
	{
		stbi_set_flip_vertically_on_load(true);

		RawTexture rawData;

		// Load texture.
		rawData.data = reinterpret_cast<char*>(stbi_load(_resourcePath.c_str(),
			reinterpret_cast<int32*>(&rawData.width),
			reinterpret_cast<int32*>(&rawData.height),
			reinterpret_cast<int32*>(&rawData.channelNum),
			_importInfos.channelNum
		));


		if (!rawData.data)
		{
			SA_LOG("Failed to load texture image!", Error, SDK_Asset);
			return false;
		}


		rawData.format = _importInfos.format;
		rawData.channelNum = _importInfos.channelNum;


		if(_importInfos.mipLevels == uint32(-1))
			rawData.mipLevels = Mipmap::ComputeLevels(rawData.width, rawData.height);
		else
			rawData.mipLevels = _importInfos.mipLevels;

		GenerateMipMaps(rawData);


		_result = Move(rawData);

		return true;
	}

	bool StbiWrapper::Import(const CubemapAssetImportInfos& _assetImportInfos, CubemapAsset& _result, const CubemapImportInfos& _importInfos)
	{
		// TODO: REMOVE LATER.
		stbi_set_flip_vertically_on_load(true);


		// TODO: CLEAN LATER.
		RawCubemap rawData;

		char* data[12u]{};

		// Load textures.
		for (uint32 i = 0u; i < 12u; ++i)
		{
			data[i] = reinterpret_cast<char*>(stbi_load(_assetImportInfos.pathes[i].c_str(),
				reinterpret_cast<int32*>(&rawData.width),
				reinterpret_cast<int32*>(&rawData.height),
				reinterpret_cast<int32*>(&rawData.channelNum),
				_importInfos.channelNum
			));

			if (!data[i])
			{
				SA_LOG("Failed to load texture image!", Error, SDK_Asset);
				return false;
			}


			rawData.format = _importInfos.format;
			rawData.channelNum = _importInfos.channelNum;
		}

		// single image size.
		const uint64 size = rawData.GetMainSize() * bitSize;

		rawData.data = reinterpret_cast<char*>(Allocate(6u * size));
		rawData.irradiancemapData = reinterpret_cast<char*>(Allocate(6u * size));

		for (uint32 i = 0u; i < 6u; ++i)
		{
			MemMove(data[i], rawData.data + i * size, size);
			MemMove(data[i + 6u], rawData.irradiancemapData + i * size, size);

			Free(data[i]);
			Free(data[i + 6u]);
		}

		// TODO: FIX LATER.
		rawData.mipLevels = RawCubemap::maxRoughLevel;
		//GenerateMipMaps(rawData);

		_result = Move(rawData);

		return true;
	}

	void StbiWrapper::GenerateMipMaps(RawTexture& _rawData)
	{
		// Can't generate mipMapLevels.
		if (_rawData.mipLevels <= 1u)
		{
			_rawData.mipLevels = 1u;
			return;
		}

		GenerateMipMaps(_rawData.width, _rawData.height, _rawData.mipLevels, _rawData.data, _rawData.channelNum);
	}

	void StbiWrapper::GenerateMipMaps(RawCubemap& _rawData)
	{
		MipMapInfos* mipmapInfos = new MipMapInfos[RawCubemap::maxRoughLevel];

		uint64 totalSize = Mipmap::ComputeTotalSize(_rawData.width, _rawData.height, RawCubemap::maxRoughLevel, mipmapInfos) * _rawData.channelNum * bitSize * 6u;

		char* original = _rawData.data;
		_rawData.data = reinterpret_cast<char*>(Allocate(totalSize));

		// Move original data.
		MemMove(original, _rawData.data, _rawData.width * _rawData.height * bitSize * _rawData.channelNum * 6u);
		Free(original);

		unsigned char* src = reinterpret_cast<unsigned char*>(_rawData.data);

		for (uint32 i = 1u; i < RawCubemap::maxRoughLevel; ++i)
		{
			uint64 parentOffset = mipmapInfos[i - 1].width * mipmapInfos[i - 1].height * bitSize * _rawData.channelNum;
			uint64 offset = mipmapInfos[i].width * mipmapInfos[i].height * bitSize * _rawData.channelNum;
			unsigned char* dst = src + 6u * parentOffset;

			for (uint32 j = 0u; j < 6u; ++j)
			{
				bool res = stbir_resize_uint8(src, mipmapInfos[i - 1].width, mipmapInfos[i - 1].height, 0,
					dst, mipmapInfos[i].width, mipmapInfos[i].height, 0,
					_rawData.channelNum);

				SA_ASSERT(res, CreationFailed, SDK_Assert, L"Mip map creation failed!");
				
				dst += offset;
				src += parentOffset;
			}
		}

		delete[] mipmapInfos;
	}

	void StbiWrapper::GenerateMipMaps(uint32 _width, uint32 _height, uint32 _mipLevels, char*& _data, uint32 _channelNum)
	{
		MipMapInfos* mipmapInfos = new MipMapInfos[_mipLevels];

		uint64 totalSize = Mipmap::ComputeTotalSize(_width, _height, _mipLevels, mipmapInfos);

		char* original = _data;
		_data = reinterpret_cast<char*>(Allocate(totalSize * bitSize * _channelNum));

		// Move original data.
		MemMove(original, _data, _width * _height * bitSize * _channelNum);
		Free(original);

		unsigned char* src = reinterpret_cast<unsigned char*>(_data);

		for (uint32 i = 1u; i < _mipLevels; ++i)
		{
			uint32 offset = mipmapInfos[i - 1].width * mipmapInfos[i - 1].height * bitSize * _channelNum;
			unsigned char* dst = src + offset;

			bool res = stbir_resize_uint8(src, mipmapInfos[i - 1].width, mipmapInfos[i - 1].height, 0,
				dst, mipmapInfos[i].width, mipmapInfos[i].height, 0,
				_channelNum);

			SA_ASSERT(res, CreationFailed, SDK_Assert, L"Mip map creation failed!");

			src = dst;
		}

		delete[] mipmapInfos;
	}
}
