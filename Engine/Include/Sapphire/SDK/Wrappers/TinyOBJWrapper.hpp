// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_TINY_OBJ_WRAPPER_GUARD
#define SAPPHIRE_SDK_TINY_OBJ_WRAPPER_GUARD

#include <string>

namespace Sa
{
	class MeshAsset;
	class MeshImportInfos;

	//class ModelAsset;
	//class ModelImportInfos;
	//class MaterialImportInfos;

	class TinyOBJWrapper
	{
		struct Callback;

		static bool Import_Internal(Callback& _cb, const std::string& _resourcePath);

	public:
		static bool ImportOBJ(const std::string& _resourcePath, MeshAsset& _result, const MeshImportInfos& _importInfos);

		//static bool ImportOBJ(const std::string& _resourcePath, ModelAsset& _result, const ModelImportInfos& _importInfos);
		//static bool ImportMTL(const std::string& _resourcePath, ModelAsset& _result, const RenderMaterialImportInfos& _importInfos);
	};
}

#endif // GUARD
