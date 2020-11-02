// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_TINY_GLTF_WRAPPER_GUARD
#define SAPPHIRE_SDK_TINY_GLTF_WRAPPER_GUARD

#include <string>

namespace Sa
{
	class ModelAsset;
	class ModelImportInfos;

	class TinyGLTFWrapper
	{
		static bool Import_Internal(const std::string& _resourcePath, ModelAsset& _result, const ModelImportInfos& _importInfos, bool _bIsBinary);

	public:
		static bool ImportGLTF(const std::string& _resourcePath, ModelAsset& _result, const ModelImportInfos& _importInfos);
		static bool ImportGLB(const std::string& _resourcePath, ModelAsset& _result, const ModelImportInfos& _importInfos);
	};
}

#endif // GUARD
