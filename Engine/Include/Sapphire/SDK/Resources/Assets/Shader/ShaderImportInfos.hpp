// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_SHADER_IMPORT_INFOS_GUARD
#define SAPPHIRE_SDK_SHADER_IMPORT_INFOS_GUARD

#include <SDK/Resources/Assets/IAssetImportInfos.hpp>

namespace Sa
{
	struct ShaderImportInfos : public IAssetImportInfos
	{
		std::string outFilePath;

		SA_ENGINE_API ShaderImportInfos() = default;
		SA_ENGINE_API ShaderImportInfos(const std::string& _outFilePath, bool _bKeepLoaded = true) noexcept;
	};
}

#endif // GUARD
