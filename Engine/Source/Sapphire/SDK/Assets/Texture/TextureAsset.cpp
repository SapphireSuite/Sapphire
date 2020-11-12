// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Texture/TextureAsset.hpp>

#include <SDK/Wrappers/StbiWrapper.hpp>

namespace Sa
{
	const RawTexture& TextureAsset::GetRawData() const noexcept
	{
		return mRawData;
	}

	void TextureAsset::FlipVertically()
	{
		StbiWrapper::FlipVertically(mRawData);
	}

	bool TextureAsset::Import(const std::string& _resourcePath, const TextureImportInfos& _importInfos)
	{
		return StbiWrapper::Import(_resourcePath, mRawData, _importInfos);
	}
}
