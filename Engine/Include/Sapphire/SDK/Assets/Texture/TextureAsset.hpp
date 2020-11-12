// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_TEXTURE_ASSET_GUARD
#define SAPPHIRE_SDK_TEXTURE_ASSET_GUARD

#include <SDK/Assets/IAsset.hpp>

#include <Rendering/Framework/Primitives/Texture/ITexture.hpp>

#include <SDK/Assets/Texture/TextureImportInfos.hpp>

namespace Sa
{
	class SA_ENGINE_API TextureAsset : public IAsset
	{
	protected:
		RawTexture mRawData;

	public:
		const RawTexture& GetRawData() const noexcept;

		void FlipVertically();
		
		bool Import(const std::string& _resourcePath, const TextureImportInfos& _importInfos = TextureImportInfos());
	};
}

#endif // GUARD
