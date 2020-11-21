// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Wrappers/StbiWrapper.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#include <Collections/Debug>

#include <Rendering/Framework/Primitives/Texture/Mipmap.hpp>
#include <Rendering/Framework/Primitives/Texture/RawTexture.hpp>
#include <Rendering/Framework/Primitives/Texture/RawCubemap.hpp>

#include <SDK/Assets/Texture/TextureImportInfos.hpp>

namespace Sa
{
	const uint32 StbiWrapper::bitSize = sizeof(stbi_uc);
	
	void StbiWrapper::FlipVertically(RawTexture& _rawData)
	{
		SA_ASSERT(_rawData.data.size(), Nullptr, SDK_Asset, L"Flip nullptr data!");

		std::vector<Vec2ui> extents(_rawData.mipLevels);

		Mipmap::ComputeTotalSize(_rawData.extent, _rawData.mipLevels, extents.data());

		uint32 offset = 0u;
		const uint32 channelNum = API_GetChannelNum(_rawData.format);

		// Flip each mipmap.
		for (uint32 i = 0; i < _rawData.mipLevels; ++i)
		{
			stbi__vertical_flip(_rawData.data.data() + offset,
				static_cast<int32>(extents[i].x),
				static_cast<int32>(extents[i].y),
				channelNum * bitSize);

			offset += extents[i].x * extents[i].y * channelNum * bitSize;
		}
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

	bool StbiWrapper::Import(const std::string& _resourcePath, RawTexture& _outTexture, const TextureImportInfos& _importInfos)
	{
		stbi_set_flip_vertically_on_load(true);

		uint32 channelNum = 0u;
		uint32 importChannelNum = API_GetChannelNum(_importInfos.format);

		// Load texture.
		char* data = reinterpret_cast<char*>(stbi_load(_resourcePath.c_str(),
			reinterpret_cast<int32*>(&_outTexture.extent.x),
			reinterpret_cast<int32*>(&_outTexture.extent.y),
			reinterpret_cast<int32*>(&channelNum),
			importChannelNum
		));


		if (!data)
		{
			SA_LOG("Failed to load texture image!", Error, SDK_Asset);
			return false;
		}

		_outTexture.format = _importInfos.format;
		_outTexture.data.assign(data, data + _outTexture.GetMainSize());


		if(_importInfos.mipLevels == ~uint32())
			_outTexture.mipLevels = Mipmap::ComputeLevels(_outTexture.extent);
		else
			_outTexture.mipLevels = _importInfos.mipLevels;

		GenerateMipMaps(_outTexture);

		return true;
	}

	bool StbiWrapper::Import(const std::string& _resourcePath, RawCubemap& _outCubemap, const TextureImportInfos& _importInfos)
	{
		// TODO: Implement.
		return false;
	}

	void StbiWrapper::GenerateMipMaps(RawTexture& _rawData)
	{
		// Can't generate mipMapLevels.
		if (_rawData.mipLevels <= 1u)
		{
			_rawData.mipLevels = 1u;
			return;
		}

		GenerateMipMaps(_rawData.extent, _rawData.mipLevels, _rawData.data, API_GetChannelNum(_rawData.format), 1u);
	}

	void StbiWrapper::GenerateMipMaps(RawCubemap& _rawData)
	{
		// Can't generate mipMapLevels.
		if (_rawData.mipLevels <= 1u)
		{
			_rawData.mipLevels = 1u;
			return;
		}

		GenerateMipMaps(_rawData.extent, _rawData.mipLevels, _rawData.data, API_GetChannelNum(_rawData.format), 6u);
	}

	void StbiWrapper::GenerateMipMaps(const Vec2ui& _extent, uint32 _mipLevels, std::vector<char>& _data, uint32 _channelNum, uint32 _layerNum)
	{
		std::vector<Vec2ui> extents(_mipLevels);

		const uint64 totalSize = Mipmap::ComputeTotalSize(_extent, _mipLevels, extents.data()) * _channelNum * _layerNum * bitSize;

		_data.resize(totalSize);

		unsigned char* src = reinterpret_cast<unsigned char*>(_data.data());

		for (uint32 i = 1u; i < _mipLevels; ++i)
		{
			uint64 srcLayerOffset = extents[i - 1].x * extents[i - 1].y * _channelNum * bitSize;
			uint64 currLayerOffset = extents[i].x * extents[i].y * _channelNum * bitSize;
			unsigned char* dst = src + srcLayerOffset * _layerNum;

			for (uint32 j = 0; j < _layerNum; ++j)
			{
				bool res = stbir_resize_uint8(
					src,
					static_cast<int32>(extents[i - 1].x),
					static_cast<int32>(extents[i - 1].y),
					0,
					dst,
					static_cast<int32>(extents[i].x),
					static_cast<int32>(extents[i].y),
					0,
					_channelNum
				);

				SA_ASSERT(res, CreationFailed, SDK_Assert, L"Mip map creation failed!");

				dst += currLayerOffset;
				src += srcLayerOffset;
			}
		}
	}
}
