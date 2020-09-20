// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_OBJ_LOADER_GUARD
#define SAPPHIRE_SDK_OBJ_LOADER_GUARD

#include <Sdk/Asset/Loaders/MeshCreateInfos.hpp>

namespace Sa
{
	class ObjLoader
	{
	public:
		~ObjLoader() = delete;

		static void Load(const std::string& _filePath, std::vector<MeshCreateInfos>& _meshInfos);
	};
}

#endif // GUARD