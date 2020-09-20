// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Sdk/Asset/AssetManager.hpp>

#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Rendering/Framework/Primitives/Pipeline/IMesh.hpp>
#include <Rendering/Framework/Primitives/Pipeline/ITexture.hpp>
#include <Rendering/Framework/Primitives/Pipeline/IShader.hpp>

#include <Sdk/Model.hpp>
#include <Sdk/Asset/Loaders/ObjLoader.hpp>

namespace Sa
{
	void AssetManager::Free(const IRenderInstance& _instance)
	{
		for (auto it = mShaders.begin(); it != mShaders.end(); ++it)
			it->second->Destroy(_instance);

		for (auto it = mTextures.begin(); it != mTextures.end(); ++it)
			it->second->Destroy(_instance);

		for (auto it = mMeshes.begin(); it != mMeshes.end(); ++it)
		{
			for(auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
				(*it2)->Destroy(_instance);
		}
	}

	IShader* AssetManager::Shader(const IRenderInstance& _instance, const std::string& _filePath)
	{
		// Shader already loaded.
		auto it = mShaders.find(_filePath);

		if (it != mShaders.end())
			return it->second;

		IShader* loadedShader = LoadShader(_instance, _filePath);
		
		mShaders.emplace(_filePath, loadedShader);

		return loadedShader;
	}

	ITexture* AssetManager::Texture(const IRenderInstance& _instance, const std::string& _filePath)
	{
		// Texture already loaded.
		auto it = mTextures.find(_filePath);

		if (it != mTextures.end())
			return it->second;

		ITexture* loadedTexture = LoadTexture(_instance, _filePath);

		mTextures.emplace(_filePath, loadedTexture);

		return loadedTexture;
	}

	Model AssetManager::Model(const IRenderInstance& _instance, const std::string& _filePath)
	{
		// Model already loaded.
		auto it = mMeshes.find(_filePath);

		return Model::CreateInstance(it != mMeshes.end() ? it->second : LoadModel(_instance, _filePath));
	}

	IShader* AssetManager::LoadShader(const IRenderInstance& _instance, const std::string& _filePath)
	{
		std::ifstream file(_filePath, std::ios::binary);

		SA_ASSERT(file.is_open(), InvalidParam, Rendering, L"failed to open shader file!");

		// Set cursor at end of file. file.open(filename, std::ios::ate | std::ios::binary) doesn't work.
		file.seekg(0, std::ios::end);

		uint32 fileSize = static_cast<uint32>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return IShader::CreateInstance(_instance, buffer);
	}

	ITexture* AssetManager::LoadTexture(const IRenderInstance& _instance, const std::string& _filePath)
	{
		// Load texture.
		int32 width = 0;
		int32 height = 0;
		int32 channels = 0;

		stbi_set_flip_vertically_on_load(true);

		stbi_uc* data = stbi_load(_filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		SA_ASSERT(data, InvalidParam, Rendering, L"Failed to load texture image!");


		// Create instance.
		ITexture* instance = ITexture::CreateInstance(_instance, data, width, height);


		// Free loaded resource.
		stbi_image_free(data);

		return instance;
	}

	std::vector<IMesh*> AssetManager::LoadModel(const IRenderInstance& _instance, const std::string& _filePath)
	{
		uint32 index = static_cast<uint32>(_filePath.rfind('.'));
		SA_ASSERT(index < _filePath.size(), InvalidParam, SDK, L"Invalid path");

		std::string fileExt = _filePath.substr(index + 1);

		std::vector<MeshCreateInfos> meshInfos;

		if (fileExt == "obj")
			ObjLoader::Load(_filePath, meshInfos);
		else
			SA_ASSERT(false, WrongExtension, SDK, L"Extension not supported!");


		// Create Meshes.
		std::vector<IMesh*> meshes;

		for (auto it = meshInfos.begin(); it != meshInfos.end(); ++it)
			meshes.push_back(IMesh::CreateInstance(_instance, it->vertices, it->indices));

		return meshes;
	}
}
