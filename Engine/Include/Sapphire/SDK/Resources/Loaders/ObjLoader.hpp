// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_OBJ_LOADER_GUARD
#define SAPPHIRE_SDK_OBJ_LOADER_GUARD

#include <SDK/Resources/Loaders/ModelCreateInfos.hpp>

namespace Sa
{
	class ObjLoader
	{
	public:
		~ObjLoader() = delete;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API static void Load(const std::string& _filePath, std::vector<ModelCreateInfos>& _modelInfos);
	};
}

#endif // GUARD