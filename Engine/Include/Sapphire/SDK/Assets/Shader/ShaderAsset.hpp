// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_SHADER_ASSET_GUARD
#define SAPPHIRE_SDK_SHADER_ASSET_GUARD

#include <SDK/Assets/IAsset.hpp>

#include <Rendering/Framework/Primitives/Shader/IShader.hpp>

namespace Sa
{
	class ShaderAsset : public IAsset
	{
	protected:
		RawShader mRawData;

		std::string mResourcePath;

		bool mHasRecompiled = false;

		bool PostLoadOperation(const std::string& _filePath) override final;

		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override final;
		void UnLoad_Internal(bool _bFreeResources) override final;

		void Save_Internal(std::fstream& _fStream) const override final;

		std::string GenerateTempPath(const std::string& _resourcePath);
		bool ShouldCompileShader(const std::string& _resourcePath, const std::string& _assetPath);
		void CompileShader(const std::string& _resourcePath, const std::string& _tempPath);

	public:
		using ResT = IShader;
		using RawT = RawShader;

		SA_ENGINE_API ShaderAsset(AssetManager& _manager) noexcept;
		SA_ENGINE_API ShaderAsset(AssetManager& _manager, RawT&& _raw) noexcept;
		SA_ENGINE_API ShaderAsset(ShaderAsset&& _other) noexcept;
		SA_ENGINE_API ~ShaderAsset();

		SA_ENGINE_API IShader* GetResource() const;
		SA_ENGINE_API bool IsValid() const noexcept override final;

		SA_ENGINE_API IShader* Create(const IRenderInstance& _instance) const;
		SA_ENGINE_API void Import(const std::string& _resourcePath);


		ShaderAsset& operator=(ShaderAsset&& _rhs);
		ShaderAsset& operator=(const ShaderAsset&) = delete;
	};
}

#endif // GUARD
