// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Wrappers/TinyGLTFWrapper.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>


#include <Collections/Debug>

#include <Core/Algorithms/Move.hpp>

#include <SDK/Assets/AssetManager.hpp>

namespace Sa
{
	bool TinyGLTFWrapper::Import(const std::string& _resourcePath, AssetManager& _assetMgr, IAssetImportResult& _result)
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		bool bRes = loader.LoadBinaryFromFile(&model, &err, &warn, _resourcePath.c_str());

		if (!bRes)
		{
			SA_LOG("Failed to load GLTF model!", Error, SDK_Asset);
			return false;
		}

		return true;
	}
}