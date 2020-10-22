// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_CUBEMAP_ASSET_GUARD
#define SAPPHIRE_SDK_CUBEMAP_ASSET_GUARD

#include <SDK/Assets/IAsset.hpp>

#include <Rendering/Framework/Primitives/Texture/ICubemap.hpp>

namespace Sa
{
	class CubemapAsset : public IAsset
	{
	protected:
		RawCubemap mRawData;

		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override final;
		void UnLoad_Internal(bool _bFreeResources) override final;

		void Save_Internal(std::fstream& _fStream) const override final;

	public:
		using ResT = ICubemap;
		using RawT = RawCubemap;

		SA_ENGINE_API CubemapAsset(AssetManager& _manager) noexcept;
		SA_ENGINE_API CubemapAsset(AssetManager& _manager, RawT&& _rawData) noexcept;
		SA_ENGINE_API CubemapAsset(CubemapAsset&& _other) noexcept;
		SA_ENGINE_API ~CubemapAsset();

		SA_ENGINE_API ResT* GetResource() const;
		SA_ENGINE_API bool IsValid() const noexcept override final;

		SA_ENGINE_API ResT* Create(const IRenderInstance& _instance) const;

		CubemapAsset& operator=(RawT&& _rhs);
		CubemapAsset& operator=(CubemapAsset&& _rhs);
		CubemapAsset& operator=(const CubemapAsset&) = delete;
	};
}

#endif // GUARD
