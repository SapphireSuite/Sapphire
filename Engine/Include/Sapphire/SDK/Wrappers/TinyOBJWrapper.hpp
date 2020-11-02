// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_TINY_OBJ_WRAPPER_GUARD
#define SAPPHIRE_SDK_TINY_OBJ_WRAPPER_GUARD

#include <string>

namespace Sa
{
	class ModelAsset;
	class ModelImportInfos;
	class RenderMaterialImportInfos;

	class TinyOBJWrapper
	{
	public:
		static bool ImportOBJ(const std::string& _resourcePath, ModelAsset& _result, const ModelImportInfos& _importInfos);
		static bool ImportMTL(const std::string& _resourcePath, ModelAsset& _result, const RenderMaterialImportInfos& _importInfos);
	};
}

#endif // GUARD
