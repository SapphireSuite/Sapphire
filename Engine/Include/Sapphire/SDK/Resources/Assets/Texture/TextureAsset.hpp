// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_TEXTURE_ASSET_GUARD
#define SAPPHIRE_SDK_TEXTURE_ASSET_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/Framework/Primitives/Texture/ITexture.hpp>

#include <SDK/Resources/Assets/IAsset.hpp>
#include <SDK/Resources/Assets/Texture/TextureAssetInfos.hpp>

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

		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override final;
		void UnLoad_Internal(bool _bFreeResources) override final;

		void Save_Internal(std::fstream& _fStream) const override final;
		void Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos) override final;

	public:
		using ImportInfosT = TextureImportInfos;
		using ImportResultT = TextureAsset;
		using CreateInfosT = TextureCreateInfos;

		SA_ENGINE_API TextureAsset(IResourceMgrBase& _manager) noexcept;
		SA_ENGINE_API TextureAsset(IResourceMgrBase& _manager, TextureCreateInfos&& _createInfos) noexcept;
		SA_ENGINE_API TextureAsset(TextureAsset&& _other) noexcept;
		SA_ENGINE_API ~TextureAsset();

		SA_ENGINE_API ITexture* GetResource() const;
		SA_ENGINE_API bool IsValid() const noexcept override final;

		SA_ENGINE_API void FlipVertically();

		SA_ENGINE_API ITexture* Create(const IRenderInstance& _instance) const;
		SA_ENGINE_API static TextureAsset Import(IResourceMgrBase& _manager, const std::string& _resourcePath, const TextureImportInfos& _importInfos);

		TextureAsset& operator=(TextureAsset&& _rhs);
		TextureAsset& operator=(const TextureAsset&) = delete;
	};
}

#endif // GUARD
