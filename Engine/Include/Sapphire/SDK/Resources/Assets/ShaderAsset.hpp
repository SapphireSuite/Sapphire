// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_SHADER_ASSET_GUARD
#define SAPPHIRE_SDK_SHADER_ASSET_GUARD

#include <SDK/Resources/Assets/IAsset.hpp>

#include <Rendering/Framework/Primitives/Shader/IShader.hpp>

namespace Sa
{
	class ShaderAsset : public IAsset
	{
	protected:
		char* mData = nullptr;
		uint32 mSize = 0u;

		ShaderAsset(char* _data, uint32 _size) noexcept;

		void Save_Internal(std::fstream& _fStream) const override;

		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override;
		void UnLoad_Internal(bool _bFreeResources) override;

	public:
		SA_ENGINE_API ShaderAsset() noexcept;
		SA_ENGINE_API ~ShaderAsset();

		SA_ENGINE_API IShader* Create(const IRenderInstance& _instance);

		SA_ENGINE_API static ShaderAsset Import(const std::string& _resourcePath);


		SA_ENGINE_API ShaderAsset& operator=(ShaderAsset&& _rhs);
		ShaderAsset& operator=(const ShaderAsset&) = delete;
	};
}

#endif // GUARD
