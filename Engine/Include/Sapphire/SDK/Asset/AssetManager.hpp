// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_ASSET_MANAGER_GUARD
#define SAPPHIRE_SDK_ASSET_MANAGER_GUARD

#include <string>
#include <unordered_map>

#include <Sdk/Config.hpp>

namespace Sa
{
	class IMesh;
	class IShader;
	class ITexture;
	class Model;
	class IRenderInstance;

	class AssetManager
	{
		std::unordered_map<std::string, IShader*> mShaders;
		std::unordered_map<std::string, ITexture*> mTextures;
		std::unordered_map<std::string, std::vector<IMesh*>> mMeshes;

		IShader* LoadShader(const IRenderInstance& _instance, const std::string& _filePath);

		ITexture* LoadTexture(const IRenderInstance& _instance, const std::string& _filePath);

		std::vector<IMesh*> LoadModel(const IRenderInstance& _instance, const std::string& _filePath);

	public:
		SA_ENGINE_API void Free(const IRenderInstance& _instance);

		SA_ENGINE_API IShader* Shader(const IRenderInstance& _instance, const std::string& _filePath);
		SA_ENGINE_API ITexture* Texture(const IRenderInstance& _instance, const std::string& _filePath);
		SA_ENGINE_API Model Model(const IRenderInstance& _instance, const std::string& _filePath);
	};
}

#endif // GUARD
