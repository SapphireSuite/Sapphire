// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_SHADER_ASSET_GUARD
#define SAPPHIRE_SDK_SHADER_ASSET_GUARD

#include <SDK/Resources/Assets/IAsset.hpp>
#include <SDK/Resources/Assets/Shader/ShaderAssetInfos.hpp>

#include <Rendering/Framework/Primitives/Shader/IShader.hpp>

namespace Sa
{
	class ShaderAsset : public IAsset
	{
	protected:
		char* mData = nullptr;
		uint32 mSize = 0u;

		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override final;
		void UnLoad_Internal(bool _bFreeResources) override final;

		void Save_Internal(std::fstream& _fStream) const override final;
		void Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos) override final;

	public:
		using ImportInfosT = ShaderImportInfos;
		using ImportResultT = ShaderAsset;
		using CreateInfosT = ShaderCreateInfos;

		SA_ENGINE_API ShaderAsset(IResourceMgrBase& _manager) noexcept;
		SA_ENGINE_API ShaderAsset(IResourceMgrBase& _manager, ShaderCreateInfos&& _createInfos) noexcept;
		SA_ENGINE_API ShaderAsset(ShaderAsset&& _other) noexcept;
		SA_ENGINE_API ~ShaderAsset();

		SA_ENGINE_API IShader* GetResource() const;
		SA_ENGINE_API bool IsValid() const noexcept override final;

		SA_ENGINE_API IShader* Create(const IRenderInstance& _instance) const;
		SA_ENGINE_API static ShaderAsset Import(IResourceMgrBase& _manager, const std::string& _resourcePath, const ShaderImportInfos& _importInfos);


		ShaderAsset& operator=(ShaderAsset&& _rhs);
		ShaderAsset& operator=(const ShaderAsset&) = delete;
	};
}

#endif // GUARD
