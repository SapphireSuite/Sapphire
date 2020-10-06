// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/Shader/ShaderImportInfos.hpp>

namespace Sa
{
	ShaderImportInfos::ShaderImportInfos(const std::string& _outFilePath, bool _bKeepLoaded) noexcept :
		IAssetImportInfos(_bKeepLoaded),
		outFilePath{ _outFilePath }
	{
	}
}
