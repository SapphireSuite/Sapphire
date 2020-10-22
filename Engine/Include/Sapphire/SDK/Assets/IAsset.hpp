// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_IASSET_GUARD
#define SAPPHIRE_SDK_IASSET_GUARD

#include <string>
#include <fstream>
#include <sstream>

#include <Core/Types/IInterface.hpp>

#include <SDK/Config.hpp>
#include <SDK/Assets/AssetDependency.hpp>

namespace Sa
{
	class AssetManager;

	class IAsset : public IInterface
	{
	protected:
		AssetManager& mManager;

		std::string mFilePath;

		IAsset(AssetManager& _manager, AssetType _assetType) noexcept;

		virtual bool PreLoadOperation(const std::string& _filePath);
		virtual bool PostLoadOperation(const std::string& _filePath);

		virtual bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) = 0;
		virtual void UnLoad_Internal(bool _bFreeResources) = 0;

		virtual void Save_Internal(std::fstream& _fStream) const = 0;
		
		static std::string GetResourceExtension(const std::string& _resourcePath);

	public:
		const AssetType assetType = AssetType::Unknown;

		IAsset(IAsset&&) = default;
		IAsset(const IAsset&) = delete;

		virtual ~IAsset() = default;

		SA_ENGINE_API const std::string& GetFilePath() const noexcept;

		virtual bool IsValid() const noexcept = 0;
		virtual std::vector<AssetPathDependency> GetPathDependencies() const noexcept;

		SA_ENGINE_API bool Load(const std::string& _filePath);
		void UnLoad(bool _bFreeResources = true);

		SA_ENGINE_API void Save(std::string _outFilePath = "", bool _bUpdateMgr = true);

		static std::string GetAssetDir(const std::string& _assetPath);
		static AssetType GetAssetTypeFromAsset(const std::string& _assetPath);

		IAsset& operator=(IAsset&& _rhs);
		IAsset& operator=(const IAsset&) = delete;
	};
}

#endif // GUARD
