// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/IAsset.hpp>

#include <filesystem>
#include <fstream>

namespace Sa
{
	IAsset::IAsset(AssetType _assetType) noexcept : assetType{ _assetType }
	{
	}


	bool IAsset::Load(const std::string& _filePath)
	{
		std::fstream fStream(_filePath, std::ios::binary | std::ios_base::in);

		if (!fStream.is_open())
		{
			SA_LOG("Failed to open file to load!", Warning, SDK_Asset);
			return false;
		}


		AssetType newAssetType = AssetType::Unknown;
		fStream.read(reinterpret_cast<char*>(&newAssetType), sizeof(AssetType));

		SA_ASSERT(newAssetType == assetType, InvalidParam, SDK_Asset, L"Invalid asset type!");


		bool res = Load_Internal(_filePath, fStream);

		fStream.close();

		return res;
	}
	
	void IAsset::UnLoad()
	{
		UnLoad_Internal();
	}

	void IAsset::Save(const std::string& _outFilePath)
	{
		SA_ASSERT(IsValid(), InvalidParam, SDK_Asset, L"Try to save invalid asset!");
		SA_ASSERT(!_outFilePath.empty(), InvalidParam, SDK_Asset, L"Try to save asset at invalid path!");

		// Create Directory.
		std::string dirPath;
		uint32 dirIndex = static_cast<uint32>(_outFilePath.find_last_of('/'));

		if (dirIndex != ~uint32())
			dirPath = _outFilePath.substr(0, dirIndex);

		if (!dirPath.empty())
			std::filesystem::create_directories(dirPath);


		// Create file.
		std::fstream fStream(_outFilePath, std::fstream::binary | std::fstream::out | std::fstream::trunc);
		SA_ASSERT(fStream.is_open(), InvalidParam, SDK_Asset, L"failed to open file to save!");

		fStream.write(reinterpret_cast<const char*>(&assetType), sizeof(AssetType));

		Save_Internal(fStream);

		fStream.close();
	}

	int32 IAsset::TryLoadImport(const std::string& _filePath, const std::string& _resourcePath, const IAssetImportInfos& _importInfos)
	{
		if (Load(_filePath))
			return 1;

		if (Import(_resourcePath, _importInfos))
			return 0;

		return -1;
	}
}
