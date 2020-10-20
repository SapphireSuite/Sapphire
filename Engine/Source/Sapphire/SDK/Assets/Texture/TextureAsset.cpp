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

	TextureAsset::TextureAsset(IResourceMgrBase& _manager, RawT&& _rawData) noexcept :
		IAsset(_manager, AssetType::Texture),
		mRawData{ Move(_rawData) }
	{
	}


	TextureAsset::TextureAsset(TextureAsset&& _other) noexcept :
		IAsset(Move(_other)),
		mRawData{ Move(_other.mRawData) }
	{
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
		return mRawData.data;
	}

	uint32 TextureAsset::GetDataSize() const noexcept
	{
		return StbiWrapper::GetDataSize(mRawData);
	}

	void TextureAsset::FlipVertically()
	{
		StbiWrapper::FlipVertically(mRawData.data, mRawData.width, mRawData.height, mRawData.channel);
	}


	bool TextureAsset::Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream)
	{
		// Header.
		if (!(_hStream >> mRawData.width >> mRawData.height >> reinterpret_cast<uint32&>(mRawData.channel)))
		{
			SA_LOG("Can't parse header!", Warning, SDK_Asset);
			return false;
		}

		const uint32 dataSize = GetDataSize();
		mRawData.data = reinterpret_cast<char*>(StbiWrapper::Allocate(dataSize));

		_fStream.read(mRawData.data, dataSize);

		return true;
	}

	void TextureAsset::UnLoad_Internal(bool _bFreeResources)
	{
		mRawData.width = 0;
		mRawData.height = 0;
		mRawData.channel = TextureChannel::RGBA;

		if (mRawData.data && _bFreeResources)
		{
			// Free loaded resource.
			StbiWrapper::Free(mRawData.data);
		}
	
		mRawData.data = nullptr;
	}


	void TextureAsset::Save_Internal(std::fstream& _fStream) const
	{
		SA_ASSERT(mRawData.data, Nullptr, SDK_Asset, L"Save nullptr texture asset!");

		// Header.
		_fStream << mRawData.width << ' ' << mRawData.height << ' ' << static_cast<uint32>(mRawData.channel) << '\n';

		_fStream.write(mRawData.data, GetDataSize());
	}


	ITexture* TextureAsset::Create(const IRenderInstance& _instance) const
	{
		return ITexture::CreateInstance(_instance, mRawData);
	}

	TextureAsset& TextureAsset::operator=(TextureAsset&& _rhs)
	{
		mRawData = Move(_rhs.mRawData);

		_rhs.UnLoad(false);

		return *this;
	}
}