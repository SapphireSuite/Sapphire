// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_MATERIAL_SHADERS_GUARD
#define SAPPHIRE_RENDERING_MATERIAL_SHADERS_GUARD

#include <Rendering/Framework/Primitives/Shader/ShaderType.hpp>
#include <Rendering/Framework/Primitives/Shader/SpecConstantInfos.hpp>

namespace Sa
{
	class IShader;

	struct MaterialShaderInfos
	{
		const IShader* shader = nullptr;

		SpecConstantInfos specConstants;
		
		const ShaderType type = ShaderType::Unknown;
	};

	struct MaterialShaders : public std::vector<MaterialShaderInfos>
	{
		using vector::vector;

		SA_ENGINE_API static const MaterialShaders default;
	};
}

#endif // GUARD