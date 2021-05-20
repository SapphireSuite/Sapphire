// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_IASSET_GUARD
#define SAPPHIRE_SDK_IASSET_GUARD

#include <Core/Types/IInterface.hpp>

#include <SDK/Assets/IAssetImportInfos.hpp>
#include <SDK/Assets/AssetType.hpp>

namespace Sa
{
	class SA_ENGINE_API IAsset : public IInterface
	{
	protected:
		IAsset(AssetType _assetType) noexcept;

		virtual void Save_Internal(std::fstream& _fStream) const = 0;

		virtual bool Load_Internal(const std::string& _filePath, std::fstream& _fStream) = 0;
		virtual void UnLoad_Internal() = 0;

	public:
		const AssetType assetType = AssetType::Unknown;

		virtual bool IsValid() const noexcept = 0;

		bool Load(const std::string& _filePath);
		void UnLoad();
		
		void Save(const std::string& _outFilePath);

		virtual bool Import(const std::string& _resourcePath, const IAssetImportInfos& _importInfos) = 0;

		/**
		*	\brief Try load filepath, if failed: import resourcePath.
		*
		*	\return 1 on load success, 0 on import sucess, -1 on failure.
		*/
		int32 TryLoadImport(const std::string& _filePath, const std::string& _resourcePath, const IAssetImportInfos& _importInfos);
	};
}

#endif // GUARD
