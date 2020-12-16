// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Texture/TextureAsset.hpp>

#include <fstream>

#include <Core/Algorithms/SizeOf.hpp>

#include <SDK/Wrappers/StbiWrapper.hpp>

namespace Sa
{
	TextureAsset::TextureAsset() noexcept : IAsset(AssetType::Texture)
	{
	}


	const RawTexture& TextureAsset::GetRawData() const noexcept
	{
		return mRawData;
	}

	void TextureAsset::FlipVertically()
	{
		StbiWrapper::FlipVertically(mRawData);
	}


	bool TextureAsset::IsValid() const noexcept
	{
		return !mRawData.data.empty();
	}


	bool TextureAsset::Load_Internal(const std::string& _filePath, std::fstream& _fStream)
	{
		(void)_filePath;

		_fStream.read(reinterpret_cast<char*>(&mRawData.extent), sizeof(Vec2ui));
		_fStream.read(reinterpret_cast<char*>(&mRawData.mipLevels), sizeof(uint32));
		_fStream.read(reinterpret_cast<char*>(&mRawData.format), sizeof(Format));

		// Data.
		uint32 dataSize = 0u;
		_fStream.read(reinterpret_cast<char*>(&dataSize), sizeof(uint32));

		SA_ASSERT(dataSize != 0u, InvalidParam, SDK_Asset, L"Asset load failure!");

		mRawData.data.resize(dataSize);
		_fStream.read(mRawData.data.data(), dataSize);

		return true;
	}
	
	void TextureAsset::UnLoad_Internal()
	{
		mRawData.extent = Vec2ui::Zero;
		mRawData.mipLevels = 1u;
		mRawData.format = Format::RGBA_32;

		mRawData.data.clear();
	}

	void TextureAsset::Save_Internal(std::fstream& _fStream) const
	{
		_fStream.write(reinterpret_cast<const char*>(&mRawData.extent), sizeof(Vec2ui));
		_fStream.write(reinterpret_cast<const char*>(&mRawData.mipLevels), sizeof(uint32));
		_fStream.write(reinterpret_cast<const char*>(&mRawData.format), sizeof(Format));

		// Data.
		const uint32 dataSize = SizeOf(mRawData.data);
		_fStream.write(reinterpret_cast<const char*>(&dataSize), sizeof(uint32));
		_fStream.write(mRawData.data.data(), dataSize);
	}


	bool TextureAsset::Import(const std::string& _resourcePath, const IAssetImportInfos& _importInfos)
	{
		return StbiWrapper::Import(_resourcePath, mRawData, _importInfos.As<ImportT>());
	}
}
