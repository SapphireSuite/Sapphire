// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/Texture/TextureAsset.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Collections/Debug>

#include <Core/Algorithms/SizeOf.hpp>
#include <Core/Algorithms/Move.hpp>

#include <SDK/Resources/Assets/AssetManager.hpp>

namespace Sa
{
#if SA_DEBUG

	// Supported extensions.
	constexpr const char* extensions[]{ "png", "jpg", "jpeg", "bmp", "tga" };

#endif


	TextureAsset::TextureAsset(IResourceMgrBase& _manager) noexcept : IAsset(_manager, AssetType::Texture)
	{
	}

	TextureAsset::TextureAsset(IResourceMgrBase& _manager, TextureCreateInfos&& _createInfos) noexcept :
		IAsset(_manager, AssetType::Texture, Move(_createInfos))
	{
		mWidth = _createInfos.width;
		mHeight = _createInfos.height;
		mChannel = _createInfos.channel;
		mData = _createInfos.data;
	}


	TextureAsset::TextureAsset(TextureAsset&& _other) noexcept : IAsset(Move(_other))
	{
		mWidth = _other.mWidth;
		mHeight = _other.mHeight;
		mChannel = _other.mChannel;
		mData = _other.mData;

		_other.UnLoad(false);
	}

	TextureAsset::~TextureAsset()
	{
		UnLoad_Internal(true);
	}


	ITexture* TextureAsset::GetResource() const
	{
		return mManager.As<ResourceMgr<ITexture, TextureAsset>>().Query(mFilePath);
	}

	bool TextureAsset::IsValid() const noexcept
	{
		return mData;
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


	void TextureAsset::Save_Internal(std::fstream& _fStream) const
	{
		SA_ASSERT(mData, Nullptr, SDK_Asset, L"Save nullptr texture asset!");

		// Header.
		_fStream << mWidth << ' ' << mHeight << ' ' << static_cast<uint32>(mChannel) << '\n';

		_fStream.write(mData, GetDataSize());
	}

	void TextureAsset::Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos)
	{
		IAsset::Import_Internal(_resourcePath, _importInfos);

		SA_ASSERT(!CheckExtensionSupport(_resourcePath, extensions, SizeOf(extensions)),
			InvalidParam, SDK_Import, L"Texture file extension not supported yet!");

		const TextureImportInfos& textImportInfos = _importInfos.As<TextureImportInfos>();

		mFilePath = textImportInfos.outFilePath;

		if(textImportInfos.bFlipVertically)
			stbi_set_flip_vertically_on_load(true);

		mChannel = textImportInfos.channel;

		// Load texture.
		mData = reinterpret_cast<char*>(stbi_load(_resourcePath.c_str(),
			reinterpret_cast<int32*>(&mWidth),
			reinterpret_cast<int32*>(&mHeight),
			reinterpret_cast<int32*>(&mChannel),
			static_cast<int32>(mChannel)
		));

		SA_ASSERT(mData, InvalidParam, Rendering, L"Failed to load texture image!");
	}


	ITexture* TextureAsset::Create(const IRenderInstance& _instance) const
	{
		return ITexture::CreateInstance(_instance, mData, mWidth, mHeight);
	}

	TextureAsset TextureAsset::Import(IResourceMgrBase& _manager, const std::string& _resourcePath, const TextureImportInfos& _importInfos)
	{
		TextureAsset asset(_manager);

		asset.Import_Internal(_resourcePath, _importInfos);

		return asset;
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