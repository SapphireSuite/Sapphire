// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_SHADER_GUARD
#define SAPPHIRE_RENDERING_SHADER_GUARD

#include <string>

namespace Sa
{
	enum class ShaderType
	{
		Unknown,

		Vertex,
		Fragment,
		Geometry,
		Compute
	};

	class Shader
	{
	protected:
		ShaderType mShaderType = ShaderType::Unknown;

	public:
		ShaderType GetShaderType() const;
	};
}

#endif // GUARD
