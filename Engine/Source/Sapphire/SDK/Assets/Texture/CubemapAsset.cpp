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
		return mRawData.data && mRawData.irradiancemapData;
	}


	bool CubemapAsset::Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream)
	{
		// Header.
		if (!(_hStream >>
			mRawData.width >>
			mRawData.height >>
			mRawData.mipLevels >>
			mRawData.channelNum >>
			reinterpret_cast<uint32&>(mRawData.format)))
		{
			SA_LOG("Can't parse header!", Warning, SDK_Asset);
			return false;
		}

		// TODO: Save GetTotalSize;
		const uint64 dataSize = mRawData.GetMapSize() * StbiWrapper::bitSize;
		const uint64 irrDataSize = mRawData.GetMapSize() * StbiWrapper::bitSize;

		mRawData.data = reinterpret_cast<char*>(StbiWrapper::Allocate(dataSize));
		mRawData.irradiancemapData = reinterpret_cast<char*>(StbiWrapper::Allocate(irrDataSize));

		_fStream.read(mRawData.data, dataSize);
		_fStream.read(mRawData.irradiancemapData, irrDataSize);

		return true;
	}

	void CubemapAsset::UnLoad_Internal(bool _bFreeResources)
	{
		mRawData.width = 0;
		mRawData.height = 0;
		mRawData.mipLevels = 1u;
		mRawData.channelNum = 4u;
		mRawData.format = TextureFormat::RGB;

		if (_bFreeResources)
		{
			// Free loaded resource.

			if (mRawData.data)
				StbiWrapper::Free(mRawData.data);

			if (mRawData.irradiancemapData)
				StbiWrapper::Free(mRawData.irradiancemapData);
		}

		mRawData.data = nullptr;
		mRawData.irradiancemapData = nullptr;
	}


	void CubemapAsset::Save_Internal(std::fstream& _fStream) const
	{
		// Header.
		_fStream << mRawData.width << ' ';
		_fStream << mRawData.height << ' ';
		_fStream << mRawData.mipLevels << ' ';
		_fStream << mRawData.channelNum << ' ';
		_fStream << static_cast<uint32>(mRawData.format) << '\n';

		// TODO: Save GetTotalSize;
		const uint64 dataSize = mRawData.GetMapSize() * StbiWrapper::bitSize;
		const uint64 irrDataSize = mRawData.GetMapSize() * StbiWrapper::bitSize;

		_fStream.write(mRawData.data, dataSize);
		_fStream.write(mRawData.irradiancemapData, irrDataSize);
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
