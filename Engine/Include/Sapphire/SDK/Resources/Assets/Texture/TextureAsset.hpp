// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_TEXTURE_ASSET_GUARD
#define SAPPHIRE_SDK_TEXTURE_ASSET_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/Framework/Primitives/Texture/ITexture.hpp>

#include <SDK/Resources/Assets/IAsset.hpp>
#include <SDK/Resources/Assets/Texture/TextureImportInfos.hpp>

namespace Sa
{
	class TextureAsset : public IAsset
	{
	protected:
		uint32 mWidth = 0;
		uint32 mHeight = 0;

		TextureChannel mChannel = TextureChannel::Unknown;

		char* mData = nullptr;

		uint32 GetDataSize() const noexcept;

		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override;
		void UnLoad_Internal(bool _bFreeResources) override;

		void Save_Internal(std::fstream& _fStream, const std::string& _newPath) const override;
		void Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos) override;

	public:
		using ImportInfoClass = TextureImportInfos;

		TextureAsset(AssetManager& _manager) noexcept;
		TextureAsset(TextureAsset&& _other) noexcept;
		SA_ENGINE_API ~TextureAsset();

		SA_ENGINE_API ITexture* GetResource() const;
		SA_ENGINE_API bool IsValid() const noexcept override;

		SA_ENGINE_API void FlipVertically();

		ITexture* Create(const IRenderInstance& _instance) const;
		static TextureAsset Import(AssetManager& _manager, const std::string& _resourcePath, const TextureImportInfos& _importInfos);

		TextureAsset& operator=(TextureAsset&& _rhs);
		TextureAsset& operator=(const TextureAsset&) = delete;
	};
}

#endif // GUARD
