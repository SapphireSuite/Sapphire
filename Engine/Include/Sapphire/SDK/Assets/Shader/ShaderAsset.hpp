// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_SHADER_ASSET_GUARD
#define SAPPHIRE_SDK_SHADER_ASSET_GUARD

#include <SDK/Assets/IAsset.hpp>

#include <Rendering/Framework/Primitives/Shader/IShader.hpp>

#include <SDK/Assets/Shader/ShaderImportInfos.hpp>

namespace Sa
{
	class SA_ENGINE_API ShaderAsset : public IAsset
	{
	protected:
		RawShader mRawData;

		static std::string GenerateTempPath(const std::string& _resourcePath);
		static bool ShouldCompileShader(const std::string& _resourcePath, const std::string& _assetPath) noexcept;
		void CompileShader(const std::string& _resourcePath, const std::string& _tempPath);

	public:
		const RawShader& GetRawData() const noexcept;

		bool Import(const std::string& _resourcePath, const ShaderImportInfos& _importInfos = ShaderImportInfos());
	};
}

#endif // GUARD
