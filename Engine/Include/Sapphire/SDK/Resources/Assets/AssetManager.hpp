// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_ASSET_MANAGER_GUARD
#define SAPPHIRE_SDK_ASSET_MANAGER_GUARD

#include <SDK/Resources/Assets/ResourceManager.hpp>

#include <SDK/Resources/Assets/Mesh/MeshAsset.hpp>
#include <SDK/Resources/Assets/Shader/ShaderAsset.hpp>
#include <SDK/Resources/Assets/Texture/TextureAsset.hpp>
#include <SDK/Resources/Assets/Material/MaterialAsset.hpp>

namespace Sa
{
	class AssetManager
	{
		const IRenderInstance& mInstance;

	public:
		ResourceMgr<IMesh, MeshAsset> meshMgr;
		ResourceMgr<IShader, ShaderAsset> shaderMgr;
		ResourceMgr<ITexture, TextureAsset> textureMgr;
		ResourceMgr<IRenderMaterial, MaterialAsset> renderMaterialMgr;

		SA_ENGINE_API AssetManager(const IRenderInstance& _instance) noexcept;
		SA_ENGINE_API ~AssetManager();
		
		SA_ENGINE_API const IRenderInstance& GetRenderInstance() const;

		SA_ENGINE_API IRenderPrimitive* Load(const std::string& _filePath, bool _bKeepLoaded = true);
		SA_ENGINE_API IRenderPrimitive* Load(const std::string& _filePath, AssetType _assetType, bool _bKeepLoaded = true);

		SA_ENGINE_API void UnLoad(IRenderPrimitive* _primitive);
		SA_ENGINE_API void UnLoad(IRenderPrimitive* _primitive, AssetType _assetType);

		SA_ENGINE_API void Clear();
	};
}

#endif // GUARD
