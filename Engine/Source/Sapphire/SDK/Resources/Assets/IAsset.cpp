// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/IAsset.hpp>

#include <filesystem>

#include <Collections/Debug>

#include <Core/Algorithms/Move.hpp>

#include <SDK/Resources/Assets/ResourceManager.hpp>

namespace Sa
{
	IAsset::IAsset(IResourceMgrBase& _manager, AssetType _assetType) noexcept :
		mManager{ _manager },
		assetType{ _assetType }
	{
	}

	IAsset::IAsset(IResourceMgrBase& _manager, AssetType _assetType, IAssetCreateInfos&& _createInfos) noexcept : IAsset(_manager, _assetType)
	{
		mFilePath = Move(_createInfos.outFilePaths);
	}

	const std::string& IAsset::GetFilePath() const noexcept
	{
		return mFilePath;
	}

	std::vector<AssetPathDependency> IAsset::GetPathDependencies() const noexcept
	{
		return std::vector<AssetPathDependency>();
	}

	bool IAsset::PreLoadOperation(const std::string& _filePath)
	{
		mFilePath = _filePath;

		return true;
	}

	bool IAsset::PostLoadOperation(const std::string& _filePath)
	{
		(void)_filePath;

		return true;
	}

	bool IAsset::Load(const std::string& _filePath)
	{
		// Other asset loaded.
		if (IsValid())
			UnLoad();

		if (!PreLoadOperation(_filePath))
		{
			SA_LOG("Pre-load operation failed!", Warning, SDK_Asset);
			return false;
		}

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


		if (!PostLoadOperation(_filePath))
		{
			SA_LOG("Post-load operation failed!", Warning, SDK_Asset);
			return false;
		}

		return res;
	}

	void IAsset::UnLoad(bool _bFreeResources)
	{
		mFilePath.clear();
		UnLoad_Internal(_bFreeResources);
	}


	void IAsset::Save(std::string _outFilePath, bool _bUpdateMgr)
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

		auto oldPath = mFilePath;
		mFilePath = _outFilePath;

		Save_Internal(fStream);

		if(_bUpdateMgr)
			mManager.Save(this, oldPath);

		fStream.close();
	}

	void IAsset::Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos)
	{
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

	AssetType IAsset::GetAssetType(const std::string& _resourcePath)
	{
		std::fstream fStream(_resourcePath, std::ios::binary | std::ios_base::in);

		SA_ASSERT(fStream.is_open(), InvalidParam, SDK_Asset, L"Failed to open file to parse!");

		std::string header;

		bool result = std::getline(fStream, header).operator bool();

		SA_ASSERT(result, InvalidParam, SDK_Asset, L"Can't parse asset's header");

		std::istringstream hStream(header);

		int32 assetType = -1;
		result = (hStream >> assetType).operator bool();
		
		SA_ASSERT(result, InvalidParam, SDK_Asset, L"Can't parse asset's header");
		SA_ASSERT(assetType > static_cast<int32>(AssetType::Unknown) && assetType < static_cast<int32>(AssetType::Max),
			InvalidParam, SDK_Asset, L"Can't parse asset's header");

		fStream.close();

		return static_cast<AssetType>(assetType);
	}


	IAsset& IAsset::operator=(IAsset&& _rhs) 
	{
		SA_ASSERT(assetType == _rhs.assetType, InvalidParam, SDK_Asset, L"Invalid asset type!");

		mFilePath = _rhs.mFilePath;

		return *this;
	}
}
