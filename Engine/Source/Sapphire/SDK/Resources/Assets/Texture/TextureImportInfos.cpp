// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/Texture/TextureImportInfos.hpp>

namespace Sa
{
	TextureImportInfos::TextureImportInfos(const std::string& _outFilePath, TextureChannel _channel, bool _bFlipVertically, bool _bKeepLoaded) noexcept :
		IAssetImportInfos(_bKeepLoaded),
		outFilePath{ _outFilePath },
		channel{ _channel },
		bFlipVertically{ _bFlipVertically }
	{
	}
}
