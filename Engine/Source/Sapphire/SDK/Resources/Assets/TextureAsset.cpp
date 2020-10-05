// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/TextureAsset.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Collections/Debug>

#include <Core/Types/Variadics/SizeOf.hpp>

namespace Sa
{
#if SA_DEBUG

	// Supported extensions.
	constexpr const char* extensions[]{ "png", "jpg", "jpeg", "bmp", "tga" };

#endif


	TextureAsset::TextureAsset() noexcept : IAsset(AssetType::Texture)
	{
	}

	TextureAsset::TextureAsset(uint32 _width, uint32 _height, TextureChannel _channel, char* _data) noexcept :
		IAsset(AssetType::Texture),
		mWidth{ _width },
		mHeight{ _height },
		mChannel{ _channel },
		mData{ _data }
	{
	}

	TextureAsset::~TextureAsset()
	{
		UnLoad_Internal(true);
	}

	uint32 TextureAsset::GetDataSize() const noexcept
	{
		return mWidth * mHeight * static_cast<int32>(mChannel) * sizeof(stbi_uc);
	}

	void TextureAsset::FlipVertically()
	{
		SA_ASSERT(mData, Nullptr, SDK_Asset, L"Flip nullptr texture asset!");

		stbi__vertical_flip(mData, static_cast<int32>(mWidth), static_cast<int32>(mHeight), static_cast<int32>(mChannel) * sizeof(stbi_uc));
	}

	ITexture* TextureAsset::Create(const IRenderInstance& _instance)
	{
		return ITexture::CreateInstance(_instance, mData, mWidth, mHeight);
	}

	TextureAsset TextureAsset::Import(const std::string& _resourcePath, TextureChannel _channel)
	{
		SA_ASSERT(!CheckExtensionSupport(_resourcePath, extensions, SizeOf(extensions)),
			InvalidParam, SDK_Import, L"Texture file extension not supported yet!");

		// Load texture.
		int32 width = 0;
		int32 height = 0;
		int32 channels = 0;

		stbi_set_flip_vertically_on_load(true);

		stbi_uc* data = stbi_load(_resourcePath.c_str(), &width, &height, &channels, static_cast<int32>(_channel));

		SA_ASSERT(data, InvalidParam, Rendering, L"Failed to load texture image!");

		return TextureAsset(width, height, _channel, reinterpret_cast<char*>(data));
	}

	void TextureAsset::Save_Internal(std::fstream& _fStream) const
	{
		SA_ASSERT(mData, Nullptr, SDK_Asset, L"Save nullptr texture asset!");

		// Header.
		_fStream << mWidth << ' ' << mHeight << ' ' << static_cast<uint32>(mChannel) << '\n';

		_fStream.write(mData, GetDataSize());
	}

	bool TextureAsset::Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream)
	{
		// Header.
		if (!(_hStream >> mWidth >> mHeight >> reinterpret_cast<uint32&>(mChannel)))
		{
			SA_LOG("Can't parse header!", Warning, SDK_Asset);
			return false;
		}

		const uint32 dataSize = GetDataSize();
		mData = reinterpret_cast<char*>(stbi__malloc(dataSize));

		_fStream.read(mData, dataSize);

		return true;
	}

	void TextureAsset::UnLoad_Internal(bool _bFreeResources)
	{
		mWidth = 0;
		mHeight = 0;
		mChannel = TextureChannel::Unknown;

		if (mData && _bFreeResources)
		{
			// Free loaded resource.
			stbi_image_free(mData);
		}
	
		mData = nullptr;
	}


	TextureAsset& TextureAsset::operator=(TextureAsset&& _rhs)
	{
		mWidth = _rhs.mWidth;
		mHeight = _rhs.mHeight;
		mChannel = _rhs.mChannel;
		mData = _rhs.mData;

		_rhs.UnLoad(false);

		return *this;
	}
}