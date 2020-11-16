// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_CUBEMAP_ASSET_GUARD
#define SAPPHIRE_SDK_CUBEMAP_ASSET_GUARD

#include <SDK/Assets/IAsset.hpp>

#include <Rendering/Framework/Primitives/Texture/ICubemap.hpp>

#include <SDK/Assets/Texture/CubemapImportInfos.hpp>

namespace Sa
{
	class SA_ENGINE_API CubemapAsset : public IAsset
	{
	protected:
		RawCubemap mRawData;

		bool Load_Internal(const std::string& _filePath, std::fstream& _fStream) override final;
		void UnLoad_Internal() override final;

		void Save_Internal(std::fstream& _fStream) const override final;

	public:
		using ImportT = CubemapImportInfos;

		CubemapAsset() noexcept;

		const RawCubemap& GetRawData() const noexcept;

		bool IsValid() const noexcept override final;

		bool Import(const std::string& _resourcePath, const IAssetImportInfos& _importInfos = ImportT()) override final;
	};
}

#endif // GUARD
