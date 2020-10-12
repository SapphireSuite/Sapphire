// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Texture/TextureAsset.hpp>

#include <Collections/Debug>

#include <Core/Algorithms/SizeOf.hpp>
#include <Core/Algorithms/Move.hpp>

#include <SDK/Assets/AssetManager.hpp>
#include <SDK/Wrappers/StbiWrapper.hpp>

namespace Sa
{
	TextureAsset::TextureAsset(IResourceMgrBase& _manager) noexcept : IAsset(_manager, AssetType::Texture)
	{
	}

	TextureAsset::TextureAsset(IResourceMgrBase& _manager, RawDataT&& _rawData) noexcept :
		IAsset(_manager, AssetType::Texture),
		mWidth{ _rawData.width },
		mHeight{ _rawData.height },
		mChannel{ _rawData.channel },
		mData{ _rawData.data }
	{
		_rawData.width = 0u;
		_rawData.height = 0u;
		_rawData.data = nullptr;
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
		ITexture* result = nullptr;
		auto& manager = mManager.As<ResourceMgr<TextureAsset>>();

		result = manager.Query(mFilePath);

		if (result)
			return result;

		return manager.Load(*this);
	}

	bool TextureAsset::IsValid() const noexcept
	{
		return mData;
	}

	uint32 TextureAsset::GetDataSize() const noexcept
	{
		return StbiWrapper::GetDataSize(mWidth, mHeight, mChannel);
	}

	void TextureAsset::FlipVertically()
	{
		StbiWrapper::FlipVertically(mData, mWidth, mHeight, mChannel);
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
		mData = reinterpret_cast<char*>(StbiWrapper::Allocate(dataSize));

		_fStream.read(mData, dataSize);

		return true;
	}

	void TextureAsset::UnLoad_Internal(bool _bFreeResources)
	{
		mWidth = 0;
		mHeight = 0;
		mChannel = TextureChannel::RGBA;

		if (mData && _bFreeResources)
		{
			// Free loaded resource.
			StbiWrapper::Free(mData);
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


	ITexture* TextureAsset::Create(const IRenderInstance& _instance) const
	{
		return ITexture::CreateInstance(_instance, mData, mWidth, mHeight);
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