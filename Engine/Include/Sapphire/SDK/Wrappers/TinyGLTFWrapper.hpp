// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_TINY_GLTF_WRAPPER_GUARD
#define SAPPHIRE_SDK_TINY_GLTF_WRAPPER_GUARD

#include <string>

namespace Sa
{
	class ModelAsset;

	class TinyGLTFWrapper
	{
		static bool Import_Internal(const std::string& _resourcePath, ModelAsset& _result, bool _bIsBinary);

	public:
		static bool ImportGLTF(const std::string& _resourcePath, ModelAsset& _result);
		static bool ImportGLB(const std::string& _resourcePath, ModelAsset& _result);
	};
}

#endif // GUARD
