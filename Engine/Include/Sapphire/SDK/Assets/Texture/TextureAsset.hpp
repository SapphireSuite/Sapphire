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

		bool Load_Internal(const std::string& _filePath, std::fstream& _fStream) override final;
		void UnLoad_Internal() override final;

		void Save_Internal(std::fstream& _fStream) const override final;

	public:
		using ImportT = TextureImportInfos;

		TextureAsset() noexcept;

		const RawTexture& GetRawData() const noexcept;

		void FlipVertically();
		
		bool IsValid() const noexcept override final;

		bool Import(const std::string& _resourcePath, const IAssetImportInfos& _importInfos = ImportT()) override final;
	};
}

#endif // GUARD
