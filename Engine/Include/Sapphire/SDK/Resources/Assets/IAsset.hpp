// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_IASSET_GUARD
#define SAPPHIRE_SDK_IASSET_GUARD

#include <string>
#include <fstream>
#include <sstream>

#include <SDK/Config.hpp>
#include <SDK/Resources/Assets/AssetType.hpp>
#include <SDK/Resources/Assets/IAssetImportInfos.hpp>

namespace Sa
{
	class AssetManager;

	class IAsset : public IInterface
	{
	protected:
		AssetManager& mManager;

		std::string mFilePath;

		IAsset(AssetManager& _manager, AssetType _assetType) noexcept;

		virtual bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) = 0;
		virtual void UnLoad_Internal(bool _bFreeResources) = 0;

		virtual void Save_Internal(std::fstream& _fStream, const std::string& _newPath) const = 0;
		virtual void Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos) = 0;
		
		static std::string GetResourceExtension(const std::string& _resourcePath);

#if SA_DEBUG
		static bool CheckExtensionSupport(const std::string& _resourcePath, const char* const* _extensions, uint32 _extensionSize);
#endif

	public:
		const AssetType assetType = AssetType::Unknown;


		IAsset(IAsset&&) = default;
		IAsset(const IAsset&) = delete;

		virtual ~IAsset() = default;

		const std::string& GetFilePath() const noexcept;
		virtual bool IsValid() const noexcept = 0;

		bool Load(const std::string& _filePath);
		void UnLoad(bool _bFreeResources = true);

		SA_ENGINE_API void Save(std::string _outFilePath = "");

		IAsset& operator=(IAsset&& _rhs);
		IAsset& operator=(const IAsset&) = delete;
	};
}

#endif // GUARD
