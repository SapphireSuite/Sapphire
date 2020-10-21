// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_ASSET_IMPORTER_GUARD
#define SAPPHIRE_SDK_ASSET_IMPORTER_GUARD

#include <string>

#include <Core/Types/Int.hpp>

#include <SDK/Config.hpp>
#include <SDK/Assets/AssetType.hpp>
#include <SDK/Assets/Importer/AssetImportResult.hpp>
#include <SDK/Assets/Importer/CubemapAssetImportInfos.hpp>

namespace Sa
{
	class AssetManager;

	class AssetImporter
	{
		AssetManager& mManager;

		std::string GetResourceExtension(const std::string& _resourcePath);
		bool ContainExt(const std::string& ext, const char* const* _extensions, uint32 _extensionSize);

		bool TryImportModel(const std::string& _resourcePath, const std::string& _extension, IAssetImportResult& _result);
		bool TryImportTexture(const std::string& _resourcePath, const std::string& _extension, IAssetImportResult& _result);
		bool TryImportShader(const std::string& _resourcePath, const std::string& _extension, IAssetImportResult& _result);
		bool TryImportRenderMaterial(const std::string& _resourcePath, const std::string& _extension, IAssetImportResult& _result);

	public:
		AssetImporter(AssetManager& _manager) noexcept;

		SA_ENGINE_API IAssetImportResult Import(const std::string& _resourcePath);
		SA_ENGINE_API IAssetImportResult ImportCubemap(const CubemapAssetImportInfos& _importInfos);
	};
}

#endif // GUARD
