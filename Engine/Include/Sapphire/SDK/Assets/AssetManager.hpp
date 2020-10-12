// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_ASSET_MANAGER_GUARD
#define SAPPHIRE_SDK_ASSET_MANAGER_GUARD

#include <SDK/Assets/AssetImporter.hpp>
#include <SDK/Assets/ResourceManager.hpp>

#include <SDK/Assets/Mesh/MeshAsset.hpp>
#include <SDK/Assets/Shader/ShaderAsset.hpp>
#include <SDK/Assets/Texture/TextureAsset.hpp>
#include <SDK/Assets/RenderMaterial/RenderMaterialAsset.hpp>

namespace Sa
{
	class AssetManager
	{
		const IRenderInstance& mInstance;

		AssetImporter mImporter;

	public:
		ResourceMgr<MeshAsset> meshMgr;
		ResourceMgr<ShaderAsset> shaderMgr;
		ResourceMgr<TextureAsset> textureMgr;
		ResourceMgr<RenderMaterialAsset> renderMatMgr;

		SA_ENGINE_API AssetManager(const IRenderInstance& _instance) noexcept;
		SA_ENGINE_API ~AssetManager();
		
		SA_ENGINE_API const IRenderInstance& GetRenderInstance() const;

		SA_ENGINE_API IRenderPrimitive* Load(const std::string& _filePath, bool _bKeepLoaded = true);
		SA_ENGINE_API IRenderPrimitive* Load(const std::string& _filePath, AssetType _assetType, bool _bKeepLoaded = true);

		SA_ENGINE_API void UnLoad(IRenderPrimitive* _primitive);
		SA_ENGINE_API void UnLoad(IRenderPrimitive* _primitive, AssetType _assetType);

		SA_ENGINE_API IAssetImportResult Import(const std::string& _resourcePath);

		SA_ENGINE_API void Clear();
	};
}

#endif // GUARD
