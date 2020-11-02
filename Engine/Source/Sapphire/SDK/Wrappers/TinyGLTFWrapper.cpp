// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Wrappers/TinyGLTFWrapper.hpp>

#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>


#include <Collections/Debug>

#include <Core/Algorithms/Move.hpp>
#include <Core/Algorithms/SizeOf.hpp>

#include <SDK/Assets/AssetManager.hpp>

namespace Sa
{
	bool TinyGLTFWrapper::Import_Internal(const std::string& _resourcePath, ModelAsset& _result, const ModelImportInfos& _importInfos, bool _bIsBinary)
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string error;
		std::string warn;

		bool bRes = false;

		if(_bIsBinary)
			bRes = loader.LoadBinaryFromFile(&model, &error, &warn, _resourcePath.c_str());
		else
			bRes = loader.LoadASCIIFromFile(&model, &error, &warn, _resourcePath.c_str());

		// TODO: CLEAN LATER.
		if (!warn.empty())
			std::cout << warn << std::endl;

		// TODO: CLEAN LATER.
		if (!error.empty())
			std::cout << error << std::endl;

		if (!bRes)
		{
			SA_LOG("Failed to load GLTF model!", Error, SDK_Asset);
			return false;
		}

		//for (uint32 i = 0u; i < SizeOf(model.bufferViews); ++i)
		//{

		//}

		return true;
	}

	bool TinyGLTFWrapper::ImportGLTF(const std::string& _resourcePath, ModelAsset& _result, const ModelImportInfos& _importInfos)
	{
		return Import_Internal(_resourcePath, _result, _importInfos, false);
	}

	bool TinyGLTFWrapper::ImportGLB(const std::string& _resourcePath, ModelAsset& _result, const ModelImportInfos& _importInfos)
	{
		return Import_Internal(_resourcePath, _result, _importInfos, true);
	}
}