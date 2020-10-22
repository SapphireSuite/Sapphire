// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_ASSET_IMPORTER_GUARD
#define SAPPHIRE_SDK_ASSET_IMPORTER_GUARD

#include <memory>
#include <string>

#include <Core/Types/Int.hpp>

#include <SDK/Config.hpp>
#include <SDK/Assets/Importer/CubemapAssetImportInfos.hpp>

namespace Sa
{
	class IAsset;
	class AssetManager;
	class ModelAsset;
	class TextureAsset;
	class CubemapAsset;
	class ShaderAsset;
	class RenderMaterialAsset;

	class AssetImporter
	{
		AssetManager& mManager;

		std::string GetResourceExtension(const std::string& _resourcePath);
		bool ContainExt(const std::string& ext, const char* const* _extensions, uint32 _extensionSize);

		ModelAsset* TryImportModel(const std::string& _resourcePath, const std::string& _extension);
		TextureAsset* TryImportTexture(const std::string& _resourcePath, const std::string& _extension);
		ShaderAsset* TryImportShader(const std::string& _resourcePath, const std::string& _extension);
		RenderMaterialAsset* TryImportRenderMaterial(const std::string& _resourcePath, const std::string& _extension);

	public:
		AssetImporter(AssetManager& _manager) noexcept;

		SA_ENGINE_API std::unique_ptr<IAsset> Import(const std::string& _resourcePath);
		SA_ENGINE_API CubemapAsset Import(const CubemapAssetImportInfos& _importInfos);
	};
}

#endif // GUARD
