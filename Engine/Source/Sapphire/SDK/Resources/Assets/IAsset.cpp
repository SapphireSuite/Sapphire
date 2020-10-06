// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/IAsset.hpp>

#include <filesystem>

#include <Collections/Debug>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	IAsset::IAsset(AssetManager& _manager, AssetType _assetType) noexcept :
		mManager{ _manager },
		assetType{ _assetType }
	{
	}

	const std::string& IAsset::GetFilePath() const noexcept
	{
		return mFilePath;
	}

	bool IAsset::Load(const std::string& _filePath)
	{
		// Other asset loaded.
		if (IsValid())
			UnLoad();

		mFilePath = _filePath;
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


	void IAsset::Save(std::string _outFilePath)
	{
		SA_ASSERT(IsValid(), InvalidParam, SDK_Asset, L"Try to save invalid asset!");
	
		if (_outFilePath.empty())
			_outFilePath = mFilePath;

		SA_ASSERT(!_outFilePath.empty(), InvalidParam, SDK_Asset, L"Try to save asset at invalid path!");


		// Create Directory.
		std::string dirPath;
		uint32 dirIndex = _outFilePath.find_last_of('/');

		if (dirIndex != uint32 (-1))
			dirPath = _outFilePath.substr(0, dirIndex);

		if(!dirPath.empty())
			std::filesystem::create_directories(dirPath);


		// Create file.
		std::fstream fStream(_outFilePath, std::fstream::binary | std::fstream::out | std::fstream::trunc);
		SA_ASSERT(fStream.is_open(), InvalidParam, SDK_Asset, L"failed to open file to save!");

		// header.
		fStream << static_cast<uint32>(assetType) << ' ';

		Save_Internal(fStream, _outFilePath);
		mFilePath = _outFilePath;

		fStream.close();
	}


	std::string IAsset::GetResourceExtension(const std::string& _resourcePath)
	{
		uint32 extIndex = _resourcePath.find_last_of('.');

		SA_ASSERT(extIndex != uint32(-1), InvalidParam, SDK_Import, L"File path invalid extension!");
		SA_ASSERT(extIndex + 1 < _resourcePath.size(), InvalidParam, SDK_Import, L"File path invalid extension!");

		return _resourcePath.substr(extIndex + 1);
	}

#if SA_DEBUG

	bool IAsset::CheckExtensionSupport(const std::string& _resourcePath, const char* const* _extensions, uint32 _extensionSize)
	{
		bool bValidExt = false;
		std::string extension = GetResourceExtension(_resourcePath);

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

		mFilePath = _rhs.mFilePath;

		return *this;
	}
}
