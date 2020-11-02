// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_ASSET_IMPORTER_GUARD
#define SAPPHIRE_SDK_ASSET_IMPORTER_GUARD

#include <memory>
#include <string>

#include <Core/Types/Int.hpp>

#include <SDK/Config.hpp>
#include <SDK/Assets/Importer/CubemapAssetImportInfos.hpp>

// TODO: REMOVE LATER?
#include <SDK/Assets/Texture/CubemapImportInfos.hpp>

namespace Sa
{
	class IAsset;
	class AssetManager;
	class ModelAsset;
	class TextureAsset;
	class CubemapAsset;
	class ShaderAsset;
	class RenderMaterialAsset;

	class IAssetImportInfos;
	class CubemapImportInfos;

	class AssetImporter
	{
		AssetManager& mManager;

		std::string GetResourceExtension(const std::string& _resourcePath);
		bool ContainExt(const std::string& ext, const char* const* _extensions, uint32 _extensionSize);

		ModelAsset* TryImportModel(const std::string& _resourcePath, const std::string& _extension, const IAssetImportInfos* _importInfos);
		TextureAsset* TryImportTexture(const std::string& _resourcePath, const std::string& _extension, const IAssetImportInfos* _importInfos);
		ShaderAsset* TryImportShader(const std::string& _resourcePath, const std::string& _extension, const IAssetImportInfos* _importInfos);
		RenderMaterialAsset* TryImportRenderMaterial(const std::string& _resourcePath, const std::string& _extension, const IAssetImportInfos* _importInfos);

	public:
		AssetImporter(AssetManager& _manager) noexcept;

		SA_ENGINE_API std::unique_ptr<IAsset> Import(const std::string& _resourcePath, const IAssetImportInfos* _importInfos = nullptr);
		SA_ENGINE_API CubemapAsset Import(const CubemapAssetImportInfos& _assetImportInfos, const CubemapImportInfos& _importInfos = CubemapImportInfos());
	};
}

#endif // GUARD
