// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Texture/CubemapAsset.hpp>

#include <fstream>

#include <Core/Algorithms/SizeOf.hpp>

#include <SDK/Wrappers/StbiWrapper.hpp>

namespace Sa
{
	CubemapAsset::CubemapAsset() noexcept : IAsset(AssetType::Cubemap)
	{
	}


	const RawCubemap& CubemapAsset::GetRawData() const noexcept
	{
		return mRawData;
	}

	bool CubemapAsset::IsValid() const noexcept
	{
		return !mRawData.data.empty();
		//return !mRawData.data.empty() && !mRawData.irradiancemapData.empty(); //TODO: Reset.
	}


	bool CubemapAsset::Load_Internal(const std::string& _filePath, std::fstream& _fStream)
	{
		(void)_filePath;

		_fStream.read(reinterpret_cast<char*>(&mRawData.extent), sizeof(Vec2ui));
		_fStream.read(reinterpret_cast<char*>(&mRawData.mipLevels), sizeof(uint32));
		_fStream.read(reinterpret_cast<char*>(&mRawData.format), sizeof(Format));
		_fStream.read(reinterpret_cast<char*>(&mRawData.irradianceformat), sizeof(Format));

		// Data.
		uint32 dataSize = 0u;
		_fStream.read(reinterpret_cast<char*>(&dataSize), sizeof(uint32));

		SA_ASSERT(dataSize != 0u, InvalidParam, SDK_Asset, L"Asset load failure!");

		mRawData.data.resize(dataSize);
		_fStream.read(mRawData.data.data(), dataSize);


		//TODO: Reset.
		//uint32 irrDataSize = 0u;
		//_fStream.read(reinterpret_cast<char*>(&irrDataSize), sizeof(uint32));

		//SA_ASSERT(irrDataSize != 0u, InvalidParam, SDK_Asset, L"Asset load failure!");

		//mRawData.irradiancemapData.resize(irrDataSize);
		//_fStream.read(mRawData.irradiancemapData.data(), irrDataSize);

		return true;
	}
	
	void CubemapAsset::UnLoad_Internal()
	{
		mRawData.extent = Vec2ui::Zero;
		mRawData.mipLevels = 1u;
		mRawData.format = Format::sRGBA_32;
		mRawData.irradianceformat = Format::RGBA_32;

		mRawData.data.clear();
		mRawData.irradiancemapData.clear();
	}

	void CubemapAsset::Save_Internal(std::fstream& _fStream) const
	{
		_fStream.write(reinterpret_cast<const char*>(&mRawData.extent), sizeof(Vec2ui));
		_fStream.write(reinterpret_cast<const char*>(&mRawData.mipLevels), sizeof(uint32));
		_fStream.write(reinterpret_cast<const char*>(&mRawData.format), sizeof(Format));
		_fStream.write(reinterpret_cast<const char*>(&mRawData.irradianceformat), sizeof(Format));

		// Data.
		const uint32 dataSize = SizeOf(mRawData.data);
		_fStream.write(reinterpret_cast<const char*>(&dataSize), sizeof(uint32));
		_fStream.write(mRawData.data.data(), dataSize);

		//TODO: Reset.
		//const uint32 irrDataSize = SizeOf(mRawData.irradiancemapData);
		//_fStream.write(reinterpret_cast<const char*>(&irrDataSize), sizeof(uint32));
		//_fStream.write(mRawData.irradiancemapData.data(), irrDataSize);
	}


	bool CubemapAsset::Import(const std::string& _resourcePath, const IAssetImportInfos& _importInfos)
	{
		return StbiWrapper::Import(_resourcePath, mRawData, _importInfos.As<CubemapImportInfos>());
	}
}
