// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Texture/CubemapAsset.hpp>

#include <Collections/Debug>

#include <Core/Algorithms/Move.hpp>

#include <SDK/Assets/AssetManager.hpp>
#include <SDK/Wrappers/StbiWrapper.hpp>

namespace Sa
{
	CubemapAsset::CubemapAsset(AssetManager& _manager) noexcept : IAsset(_manager, AssetType::Cubemap)
	{
	}

	CubemapAsset::CubemapAsset(AssetManager& _manager, RawT&& _rawData) noexcept :
		IAsset(_manager, AssetType::Cubemap),
		mRawData{ Move(_rawData) }
	{
	}


	CubemapAsset::CubemapAsset(CubemapAsset&& _other) noexcept :
		IAsset(Move(_other)),
		mRawData{ Move(_other.mRawData) }
	{
		_other.UnLoad(false);
	}

	CubemapAsset::~CubemapAsset()
	{
		UnLoad_Internal(true);
	}


	CubemapAsset::ResT* CubemapAsset::GetResource() const
	{
		ICubemap* result = nullptr;

		result = mManager.cubemapMgr.Query(mFilePath);

		if (result)
			return result;

		return mManager.cubemapMgr.Load(*this);
	}

	bool CubemapAsset::IsValid() const noexcept
	{
		return mRawData.cubemapData && mRawData.irradiancemapData;
	}


	bool CubemapAsset::Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream)
	{
		// Header.
		if (!(_hStream >> mRawData.width >>
			mRawData.height >>
			reinterpret_cast<uint32&>(mRawData.channel)))
		{
			SA_LOG("Can't parse header!", Warning, SDK_Asset);
			return false;
		}

		const uint64 dataSize = mRawData.GetMapSize() * StbiWrapper::bitSize;
		const uint64 irradianceDataSize = mRawData.GetIrradianceMapSize() * StbiWrapper::bitSize;

		mRawData.cubemapData = reinterpret_cast<char*>(StbiWrapper::Allocate(dataSize));
		_fStream.read(mRawData.cubemapData, dataSize);

		mRawData.irradiancemapData = reinterpret_cast<char*>(StbiWrapper::Allocate(irradianceDataSize));
		_fStream.read(mRawData.irradiancemapData, irradianceDataSize);

		return true;
	}

	void CubemapAsset::UnLoad_Internal(bool _bFreeResources)
	{
		mRawData.width = 0;
		mRawData.height = 0;
		mRawData.channel = TextureChannel::RGBA;

		if (_bFreeResources)
		{
			// Free loaded resource.

			if (mRawData.cubemapData)
				StbiWrapper::Free(mRawData.cubemapData);

			if (mRawData.irradiancemapData)
				StbiWrapper::Free(mRawData.irradiancemapData);
		}

		mRawData.cubemapData = nullptr;
		mRawData.irradiancemapData = nullptr;
	}


	void CubemapAsset::Save_Internal(std::fstream& _fStream) const
	{
		SA_ASSERT(mRawData.cubemapData && mRawData.irradiancemapData, Nullptr, SDK_Asset, L"Save nullptr cubemap asset!");

		// Header.
		_fStream << mRawData.width << ' ';
		_fStream << mRawData.height << ' ';
		_fStream << static_cast<uint32>(mRawData.channel) << '\n';

		const uint64 dataSize = mRawData.GetMapSize() * StbiWrapper::bitSize;
		const uint64 irradianceDataSize = mRawData.GetIrradianceMapSize() * StbiWrapper::bitSize;

		_fStream.write(mRawData.cubemapData, dataSize);
		_fStream.write(mRawData.irradiancemapData, irradianceDataSize);
	}


	CubemapAsset::ResT* CubemapAsset::Create(const IRenderInstance& _instance) const
	{
		return ICubemap::CreateInstance(_instance, mRawData);
	}

	CubemapAsset& CubemapAsset::operator=(RawT&& _rhs)
	{
		UnLoad(true);

		mRawData = Move(_rhs);

		return *this;
	}

	CubemapAsset& CubemapAsset::operator=(CubemapAsset&& _rhs)
	{
		UnLoad(true);

		mRawData = Move(_rhs.mRawData);

		_rhs.UnLoad(false);

		return *this;
	}
}
