// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_TEXTURE_ASSET_GUARD
#define SAPPHIRE_SDK_TEXTURE_ASSET_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/Framework/Primitives/Texture/ITexture.hpp>

#include <SDK/Resources/Assets/IAsset.hpp>

namespace Sa
{
	enum class TextureChannel
	{
		Unknown = 0,

		Grey = 1,

		GreyA,

		RGB,

		RGBA
	};

	class TextureAsset : public IAsset
	{
	protected:
		uint32 mWidth = 0;
		uint32 mHeight = 0;

		TextureChannel mChannel = TextureChannel::Unknown;

		char* mData = nullptr;

		TextureAsset(uint32 _width, uint32 _height, TextureChannel _channel, char* _data) noexcept;

		uint32 GetDataSize() const noexcept;

		void Save_Internal(std::fstream& _fStream) const override;

		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override;
		void UnLoad_Internal(bool _bFreeResources) override;

	public:
		SA_ENGINE_API TextureAsset() noexcept;
		SA_ENGINE_API ~TextureAsset();

		SA_ENGINE_API void FlipVertically();

		SA_ENGINE_API ITexture* Create(const IRenderInstance& _instance);

		SA_ENGINE_API static TextureAsset Import(const std::string& _resourcePath, TextureChannel _channel = TextureChannel::RGBA);

		SA_ENGINE_API TextureAsset& operator=(TextureAsset&& _rhs);
		TextureAsset& operator=(const TextureAsset&) = delete;
	};
}

#endif // GUARD
