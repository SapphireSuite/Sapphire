// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/IAsset.hpp>

#include <filesystem>

#include <Collections/Debug>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	IAsset::IAsset(AssetType _assetType) noexcept : assetType{ _assetType }
	{
	}

	const std::string& IAsset::GetFilePath() const
	{
		return mFilePath;
	}

	void IAsset::Save(const std::string& _filePath)
	{
		if (!_filePath.empty())
			mFilePath = _filePath;

		SA_ASSERT(!mFilePath.empty(), InvalidParam, SDK_Asset, L"Try to save asset at invalid path!");


		// Create Directory.
		std::string dirPath;
		uint32 dirIndex = _filePath.find_last_of('/');

		if (dirIndex != -1)
			dirPath = _filePath.substr(0, dirIndex);

		if(!dirPath.empty())
			std::filesystem::create_directories(dirPath);


		// Create file.
		std::fstream fStream(_filePath, std::fstream::binary | std::fstream::out | std::fstream::trunc);
		SA_ASSERT(fStream.is_open(), InvalidParam, SDK_Asset, L"failed to open file to save!");

		// header.
		fStream << static_cast<uint32>(assetType) << ' ';

		Save_Internal(fStream);

		fStream.close();
	}

	bool IAsset::Load(const std::string& _filePath)
	{
		// Other asset loaded.
		if (!mFilePath.empty() && mFilePath != _filePath)
			UnLoad();

		std::fstream fStream(_filePath, std::ios::binary | std::ios_base::in);

		if (!fStream.is_open())
		{
			SA_LOG("Failed to open file to load!", Warning, SDK_Asset);
			return false;
		}


		std::string header;

		if (!std::getline(fStream, header))
		{
			SA_LOG("Can't parse asset's header", Warning, SDK_Asset);
			return false;
		}


		std::istringstream hStream(header);

		uint32 newAssetType = uint32(-1);
		hStream >> newAssetType;

		SA_ASSERT(newAssetType == static_cast<uint32>(assetType), InvalidParam, SDK_Asset, L"Invalid asset type!");

		bool res = Load_Internal(Move(hStream), fStream);

		fStream.close();

		return res;
	}

	void IAsset::UnLoad(bool _bFreeResources)
	{
		mFilePath.clear();

		UnLoad_Internal(_bFreeResources);
	}

#if SA_DEBUG

	bool IAsset::CheckExtensionSupport(const std::string& _resourcePath, const char* const* _extensions, uint32 _extensionSize)
	{
		// Check extension support.
		uint32 extIndex = _resourcePath.find_last_of('.');

		SA_ASSERT(extIndex != -1, InvalidParam, SDK_Import, L"File path invalid extension!");

		bool bValidExt = false;
		std::string extension = _resourcePath.substr(extIndex + 1);

		for (uint32 i = 0; i < _extensionSize; ++i)
		{
			if (extension == _extensions[i])
			{
				bValidExt = true;
				break;
			}
		}

		return false;
	}

#endif

	IAsset& IAsset::operator=(IAsset&& _rhs) 
	{
		SA_ASSERT(assetType == _rhs.assetType, InvalidParam, SDK_Asset, L"Invalid asset type!");

		mFilePath = Move(_rhs.mFilePath);

		return *this;
	}
}
