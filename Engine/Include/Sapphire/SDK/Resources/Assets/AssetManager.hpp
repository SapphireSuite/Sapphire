// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_ASSET_MANAGER
#define SAPPHIRE_SDK_ASSET_MANAGER

#include <unordered_map>

#include <Sapphire/SDK/Resources/Assets/Mesh/MeshAsset.hpp>
#include <Sapphire/SDK/Resources/Assets/Shader/ShaderAsset.hpp>
#include <Sapphire/SDK/Resources/Assets/Texture/TextureAsset.hpp>
#include <Sapphire/SDK/Resources/Assets/Material/MaterialAsset.hpp>

namespace Sa
{
	class AssetManager
	{
		template<typename T>
		struct SmartPtr
		{
			T* ptr = nullptr;
			uint32 count = 0u;
		};

		const IRenderInstance& mInstance;
		const IRenderSurface& mSurface;

		std::unordered_map<std::string, SmartPtr<IMesh>> mMeshes;
		std::unordered_map<std::string, SmartPtr<IShader>> mShaders;
		std::unordered_map<std::string, SmartPtr<ITexture>> mTextures;
		std::unordered_map<std::string, SmartPtr<IRenderMaterial>> mMaterials;

	public:
		SA_ENGINE_API AssetManager(const IRenderInstance& _instance, const IRenderSurface& _surface) noexcept;
		SA_ENGINE_API ~AssetManager();
		
		SA_ENGINE_API IMesh* QueryMesh(const std::string& _filePath);
		SA_ENGINE_API IMesh* LoadMesh(const std::string& _filePath, bool _bPreLoaded = false);
		SA_ENGINE_API void UnloadMesh(IMesh* _mesh);
		SA_ENGINE_API std::vector<MeshAsset> ImportMeshes(const std::string& _resourcePath, const MeshImportInfos& _importInfos);
		SA_ENGINE_API std::vector<MeshAsset> ReImportMeshes(const std::string& _resourcePath, const MeshImportInfos& _importInfos);
		SA_ENGINE_API MeshAsset CreateMesh(MeshCreateInfos&& _createInfos);
		SA_ENGINE_API void SaveMesh(const MeshAsset& _asset, const std::string& _newPath);

		SA_ENGINE_API IShader* QueryShader(const std::string& _filePath);
		SA_ENGINE_API IShader* LoadShader(const std::string& _filePath, bool _bPreLoaded = false);
		SA_ENGINE_API void UnloadShader(IShader* _shader);
		SA_ENGINE_API ShaderAsset ImportShader(const std::string& _resourcePath, const ShaderImportInfos& _importInfos);
		SA_ENGINE_API ShaderAsset ReImportShader(const std::string& _resourcePath, const ShaderImportInfos& _importInfos);
		SA_ENGINE_API void SaveShader(const ShaderAsset& _asset, const std::string& _newPath);

		SA_ENGINE_API ITexture* QueryTexture(const std::string& _filePath);
		SA_ENGINE_API ITexture* LoadTexture(const std::string& _filePath, bool _bPreLoaded = false);
		SA_ENGINE_API void UnloadTexture(ITexture* _texture);
		SA_ENGINE_API TextureAsset ImportTexture(const std::string& _resourcePath, const TextureImportInfos& _importInfos);
		SA_ENGINE_API TextureAsset ReImportTexture(const std::string& _resourcePath, const TextureImportInfos& _importInfos);
		SA_ENGINE_API void SaveTexture(const TextureAsset& _asset, const std::string& _newPath);

		SA_ENGINE_API IRenderMaterial* QueryMaterial(const std::string& _filePath);
		SA_ENGINE_API IRenderMaterial* LoadMaterial(const std::string& _filePath, bool _bPreLoaded = false);
		SA_ENGINE_API void UnloadMaterial(IRenderMaterial* _material);
		SA_ENGINE_API std::vector<MaterialAsset> ImportMaterials(const std::string& _resourcePath, const MaterialImportInfos& _importInfos);
		SA_ENGINE_API std::vector<MaterialAsset> ReImportMaterials(const std::string& _resourcePath, const MaterialImportInfos& _importInfos);
		SA_ENGINE_API MaterialAsset CreateMaterial(MaterialCreateInfos&& _createInfos);
		SA_ENGINE_API void SaveMaterial(const MaterialAsset& _asset, const std::string& _newPath);

		SA_ENGINE_API void Clear();
	};
}

#endif // GUARD
